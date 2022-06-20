//
//  reset.c
//  GunwOS
//
//  Created by Artur Danielewski on 25.03.2020.
//  
//  Docs:
//  * Intel 82077AA Datasheet
//  * https://wiki.osdev.org/Floppy_Disk_Controller
//


#include "../shared/defaults.h"
#include "../shared/strings.h"
#include "../cmd/cmd.h"
#include "../proc/proc.h"
#include "../shared/io.h"
#include "../../../driver.h"

static uint_8 inPollingMode(const uint_16 base);

/*
    Controller reset
    
    This code represents procedure described in
    8.2 Initialization
    Intel 82077AA Datasheet

    Flowchart present in the documentation is a bit misleading
    so this code represents a slightly modified version
    of flow described in the datasheet 
*/

enum k_fdc_opStatus reset(const uint_16 base) {

    extern uint_8 fdc_irqRecv;
    fdc_irqRecv = 0;
    
    pushReg(base, REG_DOR, CLEAR(BIT_DOR_RESET));
    pushReg(base, REG_DOR, SET(BIT_DOR_RESET));

    extern uint_8 fdc_waitForInterrupt(const time_t);
    if (!fdc_waitForInterrupt(FDC_IRQDELAY_DEFAULT)) {                          // WAIT FOR INTERRUPT
        fdc_proc_error(base, OPSTATUS_INTEXP, FDC_ERR_INIT_IRQ);
        return OPSTATUS_INTEXP;
    }

    enum k_fdc_opStatus status;
    if (inPollingMode(base)) {                                                  // IF POLLING MODE ENABLED
        for (int i = 0; i < FDC_FDD_PER_CONTROLLER-1; ++i) {                    // 3 TIMES (first has been done by inPollingMode):
            uint_8 data;
            status = fdc_proc_senseInterruptStatus(base, &data, &data);         // ISSUE SENSE INTERRUPT STATUS COMMAND
            if (status != OPSTATUS_OK) {
                fdc_proc_error(base, status, FDC_ERR_INIT_SIS);
                return status;
            }
        }
    }
    
    pushReg(base, REG_CCR, RATE_500KBPS);                                       // PROGRAM DATA RATE VIA CCR

    status = fdc_cmd_configure(base,                                            // ISSUE CONFIGURE COMMAND
                               ISEEK_ENABLE,
                               FIFO_ENABLE,
                               POLL_DISABLE,
                               FDC_FIFOTHR_DEFAULT,
                               FDC_PRECOMP_DEFAULT);

    if (status == OPSTATUS_NOT_READY) {   
        fdc_proc_error(base, status, FDC_ERR_INIT_CFG);
        return status;
    }
    else if (status == OPSTATUS_INVPARAM) {
        fdc_proc_error(base, status, FDC_ERR_INIT_CFG_PARAM);
        return status;
    }
        
    status = fdc_cmd_specify(base,
                             FDC_SRT_DEFAULT,
                             FDC_HUT_DEFAULT,
                             FDC_HLT_DEFAULT,
                             DMA_DISABLE);                                      // ISSUE SPECIFY COMMAND

    if (status != OPSTATUS_OK) {                        
        fdc_proc_error(base, status, FDC_ERR_INIT_SPC);
        return status;
    }

    for (uint_8 i = 0; i < FDC_FDD_SUPPORT_COUNT; ++i) {                        // FOR EACH DRIVE:
        fdc_proc_recalibrate(base, i);                                          // RECALIBRATE (dumb)
        fdc_proc_stopMotor(base, i);
    }

    return OPSTATUS_OK;                                                         // CONTROLLER READY
}

static uint_8 inPollingMode(const uint_16 base) {
    uint_8 sr0, pcn;
    fdc_proc_senseInterruptStatus(base, &sr0, &pcn);

    if ((sr0 & RANGE_SR0_IC) == RANGE_SR0_IC) {
        return 1;
    }

    return 0;
}

/*
Configure

If you enable implied seeks, then you don't have to send Seek commands (or Sense Interrupt commands for the Seek commands).
If you leave the FIFO disabled, then it cannot buffer data -- you will get an IRQ6 or DMA request for every single byte (which needs to be serviced quickly, because the very next byte will automatically cause an overflow/underflow and error out your transfer!).
A big "threshold" such as 15 will wait 15 bytes between interrupts. So there won't be many interrupts, but you only have one byte left in the FIFO before it overflows/underflows and kills the r/w operation. thresh_val = threshold - 1.
Write precompensation is a technical thing having to do with drive head magnetics. A precomp_val of 0 tells the controller/drive to use the manufacturer default value. Use that unless you have a very good reason for setting another value.
*/

/*
Lock
Under default circumstances, every Controller Reset will disable the fifo, and set the fifo threshold to 1 (thresh_val = 0). If you change these settings with the Configure command and don't want to have to fix them after every Controller Reset, then you can send a Lock command with the lock bit turned on. You can "unset" the lock, by sending another Lock command with the lock bit turned off. Use the MT option bit as the lock bit.

Lock command = 0x94
or Unlock command = 0x14
No parameter bytes.
No interrupt.
First result byte = lock bit << 4
*/

/*
Specify
This command puts information in the controller about the next disk drive to be accessed.

Important Note: The controller only has one set of registers for this information. It does not store the information between multiple drives. So if you are switching control between two different types of drives (with different specify values) then you need to send a new Specify command every single time you select the other drive.

If your driver will be using DMA to transfer data, set the NDMA bit to 0. If using PIO mode instead, set it to 1.

Specify command = 0x3
First parameter byte = SRT_value << 4 | HUT_value
Second parameter byte = HLT_value << 1 | NDMA
No result bytes.
No interrupt.
SRT, HLT and HUT
These parameters sent with the Specify command to the controller are meant to optimize drive performance, and head lifetime.

The values are specific to the exact model, condition, and age of floppy drive installed on the system. The values sent by the driver to the controller were always meant to be adaptive. That is, your driver is theoretically supposed to keep statistics of how often Seek commands fail with the current setting of SRT. If they always work, and your driver wants to optimize performance, then it can send a new Specify command, with the SRT value reduced by 1. Then begin keeping new statistics. Similarly for HLT regarding Read/Write operations. As drives age and collect dirt, the driver would automatically compensate by seeing higher statistical error rates, and increase the values of SRT and HLT.

Keeping statistics in that way only works when the drive in question is used often. Now that internal floppy drives are nearly obsolete, it is worthless. So the current recommendation is just to use very safe values, and forget about performance.

If you look up spec sheets for individual floppy drives, they usually show a worst-case "track to track seek time" = SRT, but not the other two.

SRT = "Step Rate Time" = time the controller should wait for the head assembly to move between successive cylinders. A reasonable amount of time
to allow for this is 3ms for modern 3.5 inch floppy drives. A very safe amount would be 6 to 8ms. To calculate the value for the SRT setting from the given time, use "SRT_value = 16 - (milliseconds * data_rate / 500000)". For a 1.44 MB floppy and 8ms delay this gives "SRT_value = 16 - (8 * 500000 / 500000)" or a parameter value of 8.

HLT = "Head Load Time" = time the controller should wait between activating a head and actually performing a read/write.
A reasonable value for this is around 10ms. A very safe amount would be 30ms. To calculate the value for the HLT setting from the given time, use "HLT_value = milliseconds * data_rate / 1000000". For a 1.44 MB floppy and a 10ms delay this gives "HLT_value = 10 * 500000 / 1000000" or 5.

HUT = "Head Unload Time" = time the controller should wait before deactivating the head. To calculate the value for the HUT setting
from a given time, use "HUT_value = milliseconds * data_rate / 8000000". For a 1.44 MB floppy and a 240 mS delay this gives "HUT_value = 24 * 500000 / 8000000" or 15. However, it seems likely that the smartest thing to do is just to set the value to 0 (which is the maximum in any mode).
*/

/*
Sense Interrupt
This command's main function is to return any error code from a Seek or Recalibrate command to your driver. It also clears an internal bitflag in the controller. It is required in three circumstances that produce interrupts.

After doing a Controller Reset procedure with drive polling mode turned on.
After the completion of a Seek command (or Relative Seek).
After the completion of a Recalibrate command.
These are the only times when you should send a Sense Interrupt. You should still send them even if you have IRQs turned off in the DOR and you are using PIO polling instead. If you send Sense Interrupt commands at other times: the command will complete, return a 0x80, and then lock up the controller until you do a Reset.

Sense Interrupt command = 0x8
No parameter bytes.
No interrupt.
First result byte = sr0
Second result byte = controller's idea of the current cylinder
Note: if you try to read the result bytes without waiting for RQM to set, then you are likely to always get an incorrect result value of 0. This is also likely to get your driver out of sync with the FDC for input/output. The correct value of sr0 after a reset should be 0xC0 | drive number (drive number = 0 to 3). After a Recalibrate/Seek it should be 0x20 | drive number.

*/

/*
Recalibrate
Note: There is an error in the FDC datasheet regarding this command. Some statements say the command will try a maximum of 80 head assembly steps. In other places it says 79 steps. The value 79 is correct.

The motor needs to be on, and the drive needs to be selected. For this particular command, you do not have to wait for the command to complete before selecting a different drive, and sending another Recalibrate command to it (but the Step Rates have to match, for this to work).

It is possible for a normal 1.44M floppy to be formatted with 83 cylinders. So, theoretically, it may take two (or more) Recalibrates to move the head back to cylinder 0. It is a good idea to test bit 5 (value = 0x20) in sr0 after the Sense Interrupt, and retry the Recalibrate command if that bit is clear.

Recalibrate command = 0x7
First parameter byte = drive number = 0 to 3.
No result bytes.
The interrupt may take up to 3 seconds to arrive, so use a long timeout.
It is possible to poll the "disk active" bits in the MSR to find out when the head movement is finished. A Sense Interrupt command is required after this command completes, to clear it from being BUSY. (Multiple Sense Interrupts, if you ran multiple simultaneous Recalibrates.)
*/

/*
Seek
The motor needs to be on, and the drive needs to be selected. For this particular command, you do not have to wait for the command to complete before selecting a different drive, and sending another Seek command to it. Maximum cylinder number is 255; if the disk has more, you must use the Relative Seek command, instead. There is really no reason to ever use head 1 when seeking.

Seek command = 0xf
First parameter byte = (head number << 2) | drive number (the drive number must match the currently selected drive!)
Second parameter byte = requested cylinder number
No result bytes.
The interrupt may take up to 3 seconds to arrive, so use a long timeout.
It is possible to poll the "disk active" bits in the MSR to find out when the head movement is finished.

A Sense Interrupt command is required after this command completes, to clear it from being BUSY. (Multiple Sense Interrupts, if you ran multiple Seeks.)

Note: the controller tries to remember what cylinder each drive's heads are currently on. If you try to seek to that same cylinder, then the controller will silently ignore the command (and return a "success" value). One of the things this means is that you can get a "success" return value on a seek even if there is no media in the drive, if you happen to seek to the wrong cylinder number.

Relative seek
The normal Seek command allows you to select an absolute cylinder number from 0 to 255. It is also possible to use a "relative" seek command in all situations, and especially for drives that have more than 255 cylinders (there are none, currently).

To use a relative seek, set the MT bit to 1. To seek to higher cylinder numbers set the MFM bit -- clear MFM to seek backwards to lower cylinder numbers. If you seek past cylinder 255, there are a lot of extra complications. Otherwise, the command behaves identically to regular Seek.
*/

/*
Read/Write
Note: Remember that this is in CHS format, so the sector number starts at 1.

Read command = MT bit | MFM bit | 0x6
or Write command = MT bit | MFM bit | 0x5
First parameter byte = (head number << 2) | drive number (the drive number must match the currently selected drive!)
Second parameter byte = cylinder number
Third parameter byte = head number (yes, this is a repeat of the above value)
Fourth parameter byte = starting sector number
Fifth parameter byte = 2 (all floppy drives use 512bytes per sector)
Sixth parameter byte = EOT (end of track, the last sector number on the track)
Seventh parameter byte = 0x1b (GAP1 default size)
Eighth parameter byte = 0xff (all floppy drives use 512bytes per sector)
First result byte = sr0 status register
Second result byte = sr1 status register
Third result byte = sr2 status register
Fourth result byte = cylinder number
Fifth result byte = ending head number
Sixth result byte = ending sector number
Seventh result byte = 2
Note: if you try to read the result bytes without waiting for RQM to set, then you are likely to always get an incorrect result value of 0. This is also likely to get your driver out of sync with the FDC for input/output.

Note2: You'll see that there's no sector count parameter. Instead, the FDC figures out when to stop by the DMA signaling to the FDC that it's done, or in PIO mode, by the FDC experiencing a FIFO overrun or underrun.

Note3: Floppy media and electronics are well known for being unreliable. Any read or write command that fails should be retried at least twice, unless it was a write and failed on "write protect".
*/

/*

PIO Data Transfers
PIO data transfers can either be done using polling or interrupts. Using PIO mode can transfer data 10 percent faster than a DMA transfer, but there is an immense cost in CPU cycle requirements. However, if your OS or application is single-tasking, then there is nothing else that the CPU cycles can be used for anyway, so you may as well use PIO mode.

In general, the controller has a 16 byte buffer, and wants to send an IRQ6 whenever the buffer count reaches a "threshold" value that is set by your driver. If you are using PIO mode floppy transfers in a multitasking environment (bad idea), then the IRQ6 events should be used to fill or drain the floppy controller's buffer via some system buffer, in the interrupt handler code.

If you are using PIO mode in a singletasking environment then the IRQ6s just waste CPU cycles, and you should be using polling instead. You can usually shut the IRQs off by setting bit 3 (value = 8) of the Digital Output Register (see below). If you want, you can even toggle that bit on and off for specific commands, so that you receive some types of interrupts but not others.

To do PIO data transfers: init/reset the controller if needed (see below), select the drive if needed (see below), seek to the correct cylinder, issue a sense interrupt command, then issue the standard read/write commands. After you send the command, either poll the RQM bit in the Main Status Register to determine when the controller wants to have data moved in/out of the FIFO buffer -- or wait for an IRQ6 to do the same thing. When the transfer is complete, read the "result" bytes to see if there were any errors. See below for more detail.
*/

/*
There are 3 "Modes"
There were several generations of floppy controller chips on several generations of 80286 (and prior) computers, before the 82077AA chip existed. The 82077AA was built to emulate all of them, by setting various pins on the chip to 5 volts. The three modes are: PC-AT mode, PS/2 mode, and Model 30 mode. The most likely mode you will see on any hardware that still runs is Model 30 mode. You may find some pre-1996 Pentium machines using PS/2 mode. Sadly, most emulator programs run in PS/2 mode! In the documentation, you can ignore PC-AT mode. Or you can try to handle all three, by only using registers and commands that are identical in all 3 modes.
*/

/*
Most Commands Run "Silently"
There are only a few commands/conditions that produce interrupts: a reset (in polling mode only), Seek, Recalibrate, or one of the read/write/verify/format commands. Several commands do not produce any result bytes, either. If you want to verify that a silent command actually worked, just about the only thing you can do is use the Dumpreg command to check the current state of the controller.
*/

/*
Timing Issues
On real hardware, there are definite timing issues. Seek delays and motor delays are just what any programmer would expect. It would be nice if the drive would send an IRQ when the motor was up to speed, but it does not. Two things that you may not expect are that quite new hardware probably still needs artificial delays between outputting "command/parameter" bytes, and that you probably also need artificial delays between inputting "result" bytes. There is a bit in the MSR to test in order to know when the next byte can be sent/retrieved. It is not a good idea to simply hardcode specific delays between output/input bytes. Looping 20 times, and testing the value of the RQM bit in the MSR each time, should always be a sufficient "timeout".

However, using IO Port reads to generate delays (or polling MSR) leads to poor performance in a multitasking environment, and you may want to put the driver to sleep for the shortest possible period (microsleep), instead.
*/

/*
Reliability
In real hardware, floppy drives are extremely unreliable. On emulators, it doesn't matter -- but for code that is intended for real hardware, make sure to retry every command at least twice more after any error.
*/


/*
    Since polling is enabled after a reset of the 82077AA,
    four SENSE INTERRUPT STATUS commands need to be issued afterwards
    to clear the status flags for each drive

    Perform ONLY if drive polling is enabled
*/

/*
Registers
The floppy controller is programmed through 9 registers, which can be accessed through IO ports 0x3F0 through 0x3F7 (excluding 0x3F6). As usual on the PC architecture, some of those IO ports access different controller registers depending on whether you read from or write to them. Note that code snippets and datasheets name these registers based on their trigrams (e.g. SRA, MSR, DIR, CCR, etc.). Use standard "outb" and "inb" commands to access the registers.

All commands, parameter information, result codes, and disk data transfers go through the FIFO port. MSR contains the "busy" bitflags, that must be checked before reading/writing each byte through the FIFO. DOR controls the floppy drive motors, floppy drive "selection", and resets. The other registers contain very little information, and are typically accessed very little, if at all.

Note: IO port 0x3F6 is the ATA (hard disk) Alternate Status register, and is not used by any floppy controller.

As said above, the most common controller chip has 3 modes, and many bitflags in the registers are different (or opposite!) depending on the mode. However, all of the important registers and bitflags remain the same between modes. They are the following:
*/

/*
FIFO and Tape Drive Registers
FIFO: The FIFO register may not have a 16byte buffer in all modes, but this is a minor difference that does not really affect its operation.

TDR: The Tape Drive Register is identical in all modes, but it is useless (you will never find functional equipment that requires it).
*/

/*
 FIFO and Tape Drive Registers
FIFO: The FIFO register may not have a 16byte buffer in all modes, but this is a minor difference that does not really affect its operation.

TDR: The Tape Drive Register is identical in all modes, but it is useless (you will never find functional equipment that requires it).
*/

/*
DOR bitflag definitions
Mnemonic	bit number	value	meaning/usage
MOTD	7	0x80	Set to turn drive 3's motor ON
MOTC	6	0x40	Set to turn drive 2's motor ON
MOTB	5	0x20	Set to turn drive 1's motor ON
MOTA	4	0x10	Set to turn drive 0's motor ON
IRQ	3	8	Set to enable IRQs and DMA
RESET	2	4	Clear = enter reset mode, Set = normal operation
DSEL1 and 0	0, 1	3	"Select" drive number for next access
Note: the IRQ/DMA enable bit (bit 3, value = 8) cannot be cleared in "PS/2 mode", so for PIO transfers you should make sure to have a stubbed IRQ6 handler in place, just in case the IRQs happen anyway. The bit must be set for DMA to function.

Note2: if you want to execute a command that accesses a disk (see the command list below), then that respective disk must have its motor spinning (and up to speed), and its "select" bits must be set in the DOR, first.

Note3: toggling DOR reset state requires a 4 microsecond delay. It may be smarter to use DSR reset mode, because the hardware "untoggles" reset mode automatically after the proper delay.
*/

/*
MSR bitflag definitions
Mnemonic	bit number	value	meaning/usage
RQM	7	0x80	Set if it's OK (or mandatory) to exchange bytes with the FIFO IO port
DIO	6	0x40	Set if FIFO IO port expects an IN opcode
NDMA	5	0x20	Set in Execution phase of PIO mode read/write commands only.
CB	4	0x10	Command Busy: set when command byte received, cleared at end of Result phase
ACTD	3	8	Drive 3 is seeking
ACTC	2	4	Drive 2 is seeking
ACTB	1	2	Drive 1 is seeking
ACTA	0	1	Drive 0 is seeking
The two important bits are RQM and DIO. NDMA and BUSY are also useful in polling PIO mode. Most important is RQM, which is set when it is OK (or necessary!) to read/write data from/to the FIFO port. NDMA signals the end of the "execution phase" of a command, and the beginning of "result phase". DIO and BUSY should be checked to verify proper command termination (the end of "result phase" and beginning of "command phase").
*/

/*
    CCR and DSR
The bottom 2 bits of DSR match CCR, and setting one of them sets the other. The upper 6 bits on both DSR and CCR default to 0, and can always be set to zero safely. So, even though they have different bit definitions, you always set them with identical values, and one or the other of DSR and CCR can be ignored in any modern system. Alternately, you can always set both of them, for maximum compatibility with ancient chipsets.

The bottom 2 bits specify the data transfer rate to/from the drive. You want both bits set to zero for a 1.44MB or 1.2MB floppy drive. So generally, you want to set CCR to zero just once, after bootup (because the BIOS may not have done it, unless it booted a floppy disk).

Note: a reset procedure does not affect this register. However, if you have drives of different types on the bus which use different datarates, then you need to switch the datarate when you select the other drive. It also seems to be possible to modify this register while the FDC is in "reset state".

Note2: some tutorials seem to claim that changing/setting the datarate causes an IRQ6. This is false.

Datarates used for setting either DSR or CCR:

Datarate   value   Drive Type
1Mbps        3       2.88M
500Kbps      0       1.44M, 1.2M
Note: There is also a 300Kbps (value = 1), and a 250Kbps setting (value = 2) but they are only for utterly obsolete drives/media.
*/

/*
DIR register, Disk Change bit
This bit (bit 7, value = 0x80) is fairly useful. It gets set if the floppy door was opened/closed. Sadly, almost all the emulator programs set and clear this bit completely inappropriately (especially after a reset). Do not trust your handling of this bit until you have tested the functionality on real hardware.

Note: The datasheet is very confusing about the value of the bit, because Model 30 mode shows the bit as being inverted. But in Model 30 mode, the signal is also inverted, so it comes out the same. "False" always means the bit is cleared, and "true" always means the bit is set.

Note2: You must turn on the drive motor bit before you access the DIR register for a selected drive (you do not have to wait for the motor to spin up, though). It may also be necessary to read the register five times (discard the first 4 values) when changing the selected drive -- because "selecting" sometimes takes a little time.

Basically, you want to keep a flag for whether there is media in each drive. If Disk Change is set and there was media, the OS should get a signal that the previous media was ejected.

Once the Disk Change bit signals "true" (and you have processed that "event"), you need to try to clear the bit. The main way to clear the bit is with a successful Seek/Recalibrate to a new cylinder on the media. (A reset does not work. If the controller thinks the heads are already on the correct cylinder, it will eat a Seek command without clearing the Disk Change bit. If the heads are already on cylinder 0, a Recalibrate is also a no-op.) If the seek fails, you can be fairly certain that there is no media in the drive anymore. It is important to note that this means you should check the value of Disk Change just prior to every Seek command that you do, because otherwise you will lose any Disk Change information. This is also true for implied seeks, and relative seeks.

Apparently a small number of floppy drives also support one additional way to clear the bit -- something that Linux calls a "twaddle". Simply toggle the drive motor bit on, off, and then on again. When your driver tries to clear the Disk Change bit the first time, it can try a twaddle, and see if it works, and keep a flag if it does.
*/

/*
Programming Details
Overall, the controller needs to be initialized and reset once (see below for the steps involved). Then drives can be accessed. To access drives:

Turn on the drive's motor and select the drive, using an "outb" command to the DOR IO port.
Wait for awhile for the motor to get up to speed, using some waiting scheme.
Issue your command byte plus some parameter bytes (the "command phase") to the FIFO IO port.
Exchange data with the drive / "seek" the drive heads (the "execution phase"), on the FIFO IO port.
Get an IRQ6 at the end of the execution phase, but only if the command HAS an execution phase.
Read any "result bytes" produced by the command (the "result phase"), on the FIFO IO port.
The commands "Recalibrate", "Seek", and "Seek Relative" do not have a result phase, and require an additional "Sense Interrupt" command to be sent.
And then you are ready for the next command. See below for more detail on how to issue a command.
*/

/*
Motor Delays
Note: the Linux floppy driver sourcecode has a comment that claims that turning on the MOTC or MOTD bits in the DOR can completely lock up some systems. This claim seems unlikely?

When you turn a floppy drive motor on, it takes quite a few milliseconds to "spin up" -- to reach the (stabilized) speed needed for data transfer. The controller has electronics to handle a large variation in rotation speed, but it has its limits. If you start reading/writing immediately after the motor is turned on, "the PLL will fail to lock on to the data signal" and you will get an error.

After you are done reading (or writing), you should typically wait an additional two seconds to turn the motor off. (It may also be smart to seek the heads back to cylinder 0 just before turning the motor off.) You could leave the motor on longer, but it might give the user the idea that the floppy is still transferring data and that it's taking an awfully long time. The reason to leave the motor on is that your driver may not know if there is a queue of sector reads or writes that are going to be executed next. If there are going to be more drive accesses immediately, they won't need to wait for the motor to spin up.

The suggested delays when turning the motor on are:

300 milliseconds (for a 3.5" floppy).
500 milliseconds (for a 5.25" floppy).
These values should be more than enough for any floppy drive to spin up correctly. 50ms should be sufficient, in fact.

Another functional method is not delaying or waiting at all, but just enter a loop and keep retrying any command until it works. A 3.5 inch disk rotates once every 200ms, so each retry is effectively a delay.
*/

/*
Gap Lengths
There are two different gap lengths that are controlled by software for specifying the amount of blank space between sectors. The gap lengths are used by the floppy hardware to help find the "start of sector" markers, and to avoid problems caused by speed variations in different floppy drives (for e.g. writing a sector on a slower drive would cause the sector to take up more physical space on the disk, potentially overwriting the next sector). The first gap length (typically called "GPL1") is used when reading or writing data, and sets the length of the gap between sectors that the floppy should expect (but doesn't change this gap). The second (typically called "GPL2") is the gap length for the "format track" command, which specifies the amount of space between sectors to use. The actual gap lengths depend on many factors, but GPL1 is always a bit less than GPL2 so that the floppy hardware starts expecting the next sector near the end of the blank space.

Standard values for these gap lengths can be found in the floppy controller datasheets. However, it is possible to squeeze more sectors on each track by reducing the gap length. For example, by using a gap length of 0x1C when formatting (and 0x0C when reading/writing) instead of 0x54 (and 0x1B when reading/writing) it's possible to format a normal 1440 KB floppy disk with 21 sectors per track to create a 1680Kb floppy disk. These disks may be unreliable on very old computers (where accuracy and speed variation may be worse), but were considered reliable enough for Microsoft to distribute a version of Windows on 1680Kb floppies (Windows 95 on a set of 12 floppies). It is also possible to format 3 extra cylinders on each disk, for a total of 83.
*/

/*
Hardware Control of Bad Cylinders
The FDC subsystem has a built-in method for handling unreliable media. However, it is probably not a good idea to use it. It involves finding a bad sector on the media, and then marking the entire track or cylinder as being bad, during the formatting process.

If you try to do this, then you cannot simply seek to a cylinder. All of the cylinders get "remapped" with new "TrackID"s. Whenever you seek to a cylinder, then you need to use the ReadID command to verify that the cylinder you seeked to contains the data that you actually want. So this eliminates any possibility of using implied seeks, and adds an extra step to most read/write operations.
*/

/*
Drive Selection
Each floppy drive on the system may be a different type. When switching between accessing different types of drives, you need to fix the Specify and Datarate settings in the controller. The controller does not remember the settings on a per-drive basis. The only per-drive number that it remembers is the current cylinder.

Send the correct Datarate setting to CCR. Usually this is a 0 (1.44MB floppy drive).
If the newly selected drive is a different type than the previously selected drive (or changing from PIO to DMA mode), send a new Specify command.
Set the "drive select" bits (and the other bitflags) in DOR properly, including possibly turning on the Motor bit for the drive (if it will be accessed soon).
*/

/*
Detecting Media
The user can swap media out of a floppy drive at any moment. If your driver sends a command to the drive, and the command fails -- this may be the reason why.

Turn the drive motor bit on.
Read DIR. If the "Disk Change" bitflag is set to "true", then the floppy drive door was opened, so the OS needs to test if a new disk is in the drive.
*/

/*
Waiting
Waiting for the drive can be done in many ways, and it is an OS-specific design decision. You can poll the value of the PIT, waiting for it to count down to a certain value. You can poll a memory location that contains "the current time" in some format, waiting for it to reach a certain value. Your OS can implement a realtime callback, where a particular function in the driver will be called at a particular time. Or you can implement some form of multitasking "blocking", where the driver process is put in a "sleep" state, and is not assigned any timeslices for a certain length of time.
*/

/*
The Proper Way to issue a command
Read MSR (port 0x3F4).
Verify that RQM = 1 and DIO = 0 ((Value & 0xc0) == 0x80) -- if not, reset the controller and start all over.
Send your chosen command byte to the FIFO port (port 0x3F5).
In a loop: loop on reading MSR until RQM = 1. Verify DIO = 0, then send the next parameter byte for the command to the FIFO port.
Either Execution or Result Phase begins when all parameter bytes have been sent, depending on whether you are in PIO mode, and the command has an Execution phase. If using DMA, or the command does not perform read/write/head movement operations, skip to the Result Phase.
(In PIO Mode Execution phase) read MSR, verify NDMA = 1 ((Value & 0x20) == 0x20) -- if it's not set, the command has no Execution phase, so skip to Result phase.
begin a loop:
Either poll MSR until RQM = 1, or wait for an IRQ6, using some waiting method.
In an inner loop: transfer a byte in or out of the FIFO port via a system buffer, then read MSR. Repeat while RQM = 1 and NDMA = 1 ((Value & 0xa0) == 0xa0).
if NDMA = 1, loop back to the beginning of the outer loop, unless your data buffer ran out (detect underflow/overflow).
Result Phase begins. If the command does not have a Result phase, it silently exits to waiting for the next command.
If using DMA on a read/write command, wait for a terminal IRQ6.
Loop on reading MSR until RQM = 1, verify that DIO = 1.
In a loop: read the next result byte from the FIFO, loop on reading MSR until RQM = 1, verify CMD BSY = 1 and DIO = 1 ((Value & 0x50) == 0x50).
After reading all the expected result bytes: check them for error conditions, verify that RQM = 1, CMD BSY = 0, and DIO = 0. If not retry the entire command again, several times, starting from step 2!
Note: implementing a failure timeout for each loop and the IRQ is pretty much required -- it is the only way to detect many command errors.
*/

/*
Perpendicular Mode and 2.88M floppies
If you are using an emulator and you need a floppy disk image that is bigger than 1440Kb, there is a 2880Kb image available. In order to access it in Pmode, you need to modify your driver to handle Perpendicular Mode. Basically, it is an extra configuration command where you enable any of the four drives for perpendicular mode.

Note: If the parameter byte is 0 (except for the "perpendicular enable" bits), then a reset will not affect the settings.

Perpendicular Mode command = 0x12
First parameter byte = (Drive 3 enable << 5) | (Drive 2 enable << 4) | (Drive 1 enable << 3) | (Drive 0 enable << 2)
No result bytes.
No interrupt.
You also need to set CCR/DSR for the 1M datarate (value = 3) to access a 2.88M drive.
*/

/*
    The drive records the two sides with two "heads" that are bolted together.
    They cannot seek independently.
    The "track" on one side of a disk is always exactly opposite the track on the other side of the disk.
    (There is a misconception about this in many floppy driver code examples.)
    You only need to seek one head to find a particular cylinder, so that both heads may read/write that cylinder.
*/

/*
Additional Programming Notes
If you are doing a transfer between 2 floppy drives (so that both motors are on), and you are toggling "selection" between the two, there may be a short delay required.
*/

/*
If you don't want to bother having to send another Configure command after every Reset procedure, then:
Send a better Configure command to the controller. A suggestion would be: drive polling mode off, FIFO on, threshold = 8, implied seek on, precompensation 0.
send a Lock command.
Do a Controller Reset procedure.
Send a Recalibrate command to each of the drives.
*/

/*
Since the 125 ns write precompensation value is optimal
for the 5,25" and 3,5" disk drive environment,
most applications will not require the value to be changed
in the initialization sequence
*/

/*
Related Links
Articles
Reference Documents
http://www.osdever.net/documents/82077AA_FloppyControllerDatasheet.pdf?the_id=41
http://bos.asmhackers.net/docs/floppy/
http://bos.asmhackers.net/docs/floppy/docs/floppy_tutorial.txt
Intel 82078 CHMOS SINGLE-CHIP FLOPPY DISK CONTROLLER datasheet (useless)
http://www.brokenthorn.com/Resources/OSDev20.html
Forum Posts
TUTORIAL, with DMA, by Mystran (highly recommended, but has a few tiny errors)
Floppy programming tutorial (floppy_tutorial.txt) companion thread
*/
