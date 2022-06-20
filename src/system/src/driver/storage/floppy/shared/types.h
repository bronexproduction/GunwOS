//
//  types.h
//  GunwOS
//
//  Created by Artur Danielewski on 04.04.2020.
//

#ifndef FDC_TYPES_H
#define FDC_TYPES_H

#include "../../storage.h"
#include "../../../../../../lib/stdgunw/types.h"
#include "../../../../../include/gunwstor.h"

enum fdc_bus_regOffset {
    #warning verify pin names
    /* Register layout order: MSB - LSB */

    REG_SRA     = 0x00, /* Status Register A
    
    I/O - Read
    
    PC-AT:
        Not available
    PS/2:
        | INT_PENDING | *DRV2 | STEP     | *TRK0 |  HDSEL | *INDEX | *WP |  DIR |
    PS/2 Model 30:
        | INT_PENDING |  DRQ  | STEP F/F |  TRK0 | *HDSEL |  INDEX |  WP | *DIR |
    */

    REG_SRB     = 0x01, /* Status Register B

    I/O - Read

    PC-AT:
        Not available
    PS/2:
        | 1     | 1    |  DS0 | WRDATA | RDDATA | WE |  ME1 |  ME0 |
    PS/2 Model 30:
        | *DRV2 | *DS1 | *DS0 | WRDATA | RDDATA | WE | *DS3 | *DS2 |
    */

    REG_DOR     = 0x02, /* Digital Output Register

    I/O - Read/write

    All modes:
        | ME3 | ME2 | ME1 | ME0 | *DMA_GATE | *RESET | DS1 | DS0 |
    */

    REG_TDR     = 0x03, /* Tape Drive Register

    I/O - Read/write

    All modes:
        | - | - | - | - | - | - | TAPE_SELECT1 | TAPE_SELECT0 |
    */

    REG_DSR     = 0x04, /* Datarate Select Register

        I/O - Write

    All modes:
        | S/W RESET | POWER_DOWN | 0 | PRE-COMP2 | PRE-COMP1 | PRE-COMP0 | DRATE_SEL1 | DRATE_SEL0 |
    */

    REG_MSR     = 0x04, /* Main Status Register

        I/O - Read

    All modes:
        | RQM | DIO | NON_DMA | CMD_BSY | DRV3_BUSY | DRV2_BUSY | DRV1_BUSY | DRV0_BUSY |
    */

    REG_DATA    = 0x05, /* FIFO

        I/O - Read/write

    All modes:
        All bytes are just data
    */

    REG_DIR     = 0x07, /* Digital Input Register

        I/O - Read

    PC-AT:
        |  DSK_CHG | - | - | - |  -        | -          | -          |  -          |
    PS/2:
        |  DSK_CHG | 1 | 1 | 1 |  1        | DRATE_SEL1 | DRATE_SEL0 | *HIGH_DENS  | 
    PS/2 Model 30:
        | *DSK_CHG | 0 | 0 | 0 | *DMA_GATE | NOPREC     | DRATE_SEL1 |  DRATE_SEL0 |
    */

    REG_CCR     = 0x07  /* Configuration Control Register 

        I/O - Write

    PC-AT:
        | - | - | - | - | - | -      | DRATE_SEL1 | DRATE_SEL0 |
    PS/2:
        | - | - | - | - | - | -      | DRATE_SEL1 | DRATE_SEL0 |
    PS/2 Model 30:
        | - | - | - | - | - | NOPREC | DRATE_SEL1 | DRATE_SEL0 |
    */
};

enum fdc_bus_regBit {

    /*
        Status Register A bits (SRA)
    */

    /*
        INT PENDING

        The INT PENDING bit is used by software to monitor
        the state of the 82077AA INTERRUPT pin
    */
    BIT_SRA_INT_PENDING = 0x80,

    /*
        *DRV2/DRQ

        PS/2:

            *DRV2 - This indicates whether a second drive is installed
        
        PS/2 Model 30:
            
            DRQ - Requests service from a DMA controller. 
            Normally active high, but goes to high impedance in AT and Model 30 modes
            when the appropriate bit is set in the DOR.
    */
    BIT_SRA_DRV2        = 0x40,
    BIT_SRA_DRQ         = 0x40,

    /*
        STEP/STEP F/F

        PS/2:

            STEP - Supplies step pulses to the drive
        
        PS/2 Model 30:
            
            STEP F/F - Supplies step pulses to the drive. Latched with the Step output going active
            and cleared with a read to the DIR register, Hardware or Software RESET
    */
    BIT_SRA_STEP        = 0x20,

    /*
        *TRK0/TRK0

        Control line that indicates that the head is on track 0

        PS/2:          *TRK0
        PS/2 Model 30:  TRK0
    */
    BIT_SRA_TRK0        = 0x10,

    /*
        HDSEL / *HDSEL

        Selects which side of a disk is to be used.
        An active level selects side 1.

        PS/2:           HDSEL
        PS/2 Model 30: *HDSEL
    */
    BIT_SRA_HDSEL       = 0x08,

    /*
        *INDX/INDX

        Indicates the beginning of the track

        PS/2:          *INDX
        PS/2 Model 30:  INDX
    */
    BIT_SRA_INDEX       = 0x04,

    /*
        *WP/WP

        Indicates whether the disk drive is write protected

        PS/2:          *WP
        PS/2 Model 30:  WP
    */
    BIT_SRA_WP          = 0x02,

    /*
        DIR / *DIR

        Controls the direction the head moves when a step signal is present.
        The head moves toward the center if active.

        PS/2:           DIR
        PS/2 Model 30: *DIR
    */
    BIT_SRA_DIR         = 0x01,

    /*
        Status Register B bits (SRB)
    */

    /*
        *DRV2 (PS/2 Model 30 mode only)

        This indicates whether a second drive is installed
    */
    BIT_SRB_DRV2        = 0x80,

    /*
        *DS1 (PS/2 Model 30 mode only)

        DRIVE SELECT 1
    */
    BIT_SRB_DS1         = 0x40,

    /*
        DS0 / *DS0

        DRIVE SELECT 0

        PS/2:           DS0
        PS/2 Model 30: *DS0
    */
    BIT_SRB_DS0         = 0x20,

    /*
        WRDATA/WRDATA F/F

        FM or MFM serial data to the drive. 
        Precompensation value is selectable through software

        PS/2:           WRDATA 

        PS/2 Model 30:
        
            WRDATA F/F
            
            The WRDATA bit is triggered by raw WRDATA signals
            and is not gated by WE.
    */
    BIT_SRB_WRDATA      = 0x10,

    /*
        RDDATA/RDDATA F/F

        Serial data from the disk.
        INVERT also affects the polarity of this signal.

        PS/2:
        
            RDDATA 

            As the only drive input, RDDATA TOGGLE’s activity
            is independent of the INVERT pin level and reflects
            the level as seen on the cable.

        PS/2 Model 30:  RDDATA F/F
    */
    BIT_SRB_RDDATA      = 0x08,

    /*
        WE/WE F/F

        WRITE ENABLE: Drive control signal that enables the head to write onto the disk.

        PS/2:           WE 

        PS/2 Model 30: 
        
            WE F/F
        
            Bit WE is cleared to a low level
            by either Hardware or Software RESET.
    */
    BIT_SRB_WE          = 0x04,

    /*
        ME1 / *DS3

        PS/2:           ME1
        
            MOTOR ENABLE 1

        PS/2 Model 30: *DS3

            DRIVE SELECT 3
    */
    BIT_SRB_ME1         = 0x02,
    BIT_SRB_DS3         = 0x02,

    /*
        ME0 / *DS2

        PS/2:           ME0
        
            MOTOR ENABLE 0

        PS/2 Model 30: *DS2

            DRIVE SELECT 2
    */
    BIT_SRB_ME0         = 0x01,
    BIT_SRB_DS2         = 0x01,

    /*
        Digital Output Register bits (DOR)
    */

    /*
        MOTOR ENABLE 3-0

        The MOT ENx bits directly control their respective
        motor enable pins (ME0 –3). A one means the pin is
        active, the INVERT pin determines the active level.
        The DRIVE SELx bits are decoded to provide four
        drive select lines and only one may be active at a
        time. A one is active and the INVERT pin determines
        the level on the cable. 
        Standard programming practice is to set
        both MOT ENx and DRIVE SELx bits at the same time.
    */
    BIT_DOR_ME3         = 0x80,
    BIT_DOR_ME2         = 0x40,
    BIT_DOR_ME1         = 0x20,
    BIT_DOR_ME0         = 0x10,

    /*
        *DACK

        DMA ACKNOWLEDGE: Control input that qualifies the RD, WR inputs in DMA cycles.
        Normally active low, but is disabled in AT and Model 30 modes when the appropriate bit
        is set in the DOR.e

        The DMAGATE bit is enabled only in PC-AT and
        Model 30 Modes. If DMAGATE is set low,
        the INT and DRQ outputs are tristated
        and the DACK and TC inputs are disabled. 
        DMAGATE set high will enable INT, DRQ, TC, and DACK to the system. 
        In PS/2 Mode DMAGATE has no effect upon INT, DRQ,
        TC or DACK pins and they are always active.
    */
    BIT_DOR_DACK        = 0x08,

    /*
        *RESET

        Triggers software reset

        The DOR Reset is set automatically upon a pin RESET
        The user must manually clear this reset bit in the DOR to exit the reset state
    */
    BIT_DOR_RESET       = 0x04,

    /*
        DRIVE SELECT 1-0
    */
    BIT_DOR_DS1         = 0x02,
    BIT_DOR_DS0         = 0x01,
   
    /*
        Datarate Select Register bits (DSR)
    */

    /*
        S/W RESET
        
        S/W RESET behaves the same as DOR RESET except that this reset is self clearing.
    */
    BIT_DSR_SWR         = 0x80,

    /*
        POWER DOWN
        
        POWER DOWN deactivates the internal clocks and shuts off the oscillator. 
        Disk control pins are put in an inactive state. 
        All input signals must be held in a valid state (D.C. level 1 or 0). 
        POWER DOWN is exited by activating one of the reset functions
    */
    BIT_DSR_PWRDOWN     = 0x40,

    /*
        Status Register 0 bits mapping (register read from FIFO)
    */

    /*
        SEEK END

        The FDC completed a SEEK, RELATIVE SEEK or RECALIBRATE command
        (used during a SENSE INTERRUPT command)
    */
    BIT_SR0_SE          = 0x20,

    /*
        EQUIPMENT CHECK

        The TRK0 pin failed to become a "1" after:
        1. 80 step pulses in the Recalibrate command
        2. The RELATIVE SEEK command caused the FDC to step outward beyond Track 0
    */
    BIT_SR0_EC          = 0x10,

    /*
        HEAD ADDRESS

        The current head address
    */
    BIT_SR0_H           = 0x04,

    /*
        DRIVE SELECT 1-0
    */
    BIT_SR0_DS1         = 0x02,
    BIT_SR0_DS0         = 0x01,
   
    /*
        Status Register 1 bits mapping (register read from FIFO)
    */

    /*
        END OF CYLINDER

        The FDC tried to access a sector beyond the final sector of the track (255).
        Will be set if TC is not issued after READ or WRITE DATA command
    */
    BIT_SR1_EN          = 0x80,

    /*
        DATA ERROR

        The FDC detected a CRC error in either the ID field or the data field of a sector
    */
    BIT_SR1_DE          = 0x20,

    /*
        OVERRUN/UNDERRUN

        Becomes set if the FDC does not receive CPU or DMA service
        within the required time interval, resulting in data overrun or underrun
    */
    BIT_SR1_OR          = 0x10,

    /*
        NO DATA

        Any one of the following:
        1. READ DATA, READ DELETED DATA command - the FDC did not find the specified sector
        2. READ ID command - the FDC cannot read the ID field without an error
        3. READ TRACK command - the FDC cannot find the proper sector sequence
    */
    BIT_SR1_ND          = 0x04,

    /*
        NOT WRITABLE

        WP pin became a "1" 
        while the FDC is executing a WRITE DATA, WRITE DELETED DATA or FORMAT TRACK command
    */
    BIT_SR1_NW          = 0x02,

    /*
        MISSING ADDRESS MARK

        Any one of the following:
        1. The FDC did not detect an ID address mark at the specified track
           after encountering the index pulse from the IDX pin twice
        2. The FDC cannot detect a data address mark
           or a deleted data address mark on the specified track
    */
    BIT_SR1_MA          = 0x01,
   
    /*
        Status Register 2 bits mapping (register read from FIFO)
    */

    /*
        CONTROL MARK

        Any of the following:
        1. READ DATA command - the FDC encountered a deleted data address mark
        2. READ DELETED DATA command - the FDC encountered a data address mark
    */
    BIT_SR2_CM          = 0x40,

    /*
        DATA ERROR IN DATA FIELD

        The FDC detected a CRC error in the data field
    */
    BIT_SR2_DD          = 0x20,

    /*
        WRONG CYLINDER

        The track address from the sector ID field is different
        from the track address maintained inside the FDC
    */
    BIT_SR2_WC          = 0x10,

    /*
        BAD CYLINDER

        The track address from the sector ID field is different
        from the track address maintained inside the FDC
        and is equal to 0xFF, which indicates a bad track with a hard error
        according to the IBM soft-sectored format
    */
    BIT_SR2_BC          = 0x02,

    /*
        MISSING DATA ADDRESS MARK

        The FDC cannot detect a data address mark or a deleted data address mark
    */
    BIT_SR2_MD          = 0x01,
   
    /*
        Status Register 3 bits mapping (register read from FIFO)
    */

    /*
        WRITE PROTECTED

        Indicates the status of the WP pin
    */
    BIT_SR3_WP          = 0x40,

    /*
        TRACK 0

        Indicates the status of the TRK0 pin
    */
    BIT_SR3_T0          = 0x10,

    /*
        HEAD ADDRESS

        Indicates the status of the HDSEL pin
    */
    BIT_SR3_HD          = 0x04,
   
    /*
        Main Status Register bits (MSR)
    */

    /*
        RQM

        Indicates that the host can transfer data if set to a 1.
        No access is permitted if set to a 0.
    */
    BIT_MSR_RQM     = 0x80,

    /*
        DIO
        
        Indicates the direction of a data transfer once RQM is set.
        A 1 indicates a read and a 0 indicates a write is required.
    */
    BIT_MSR_DIO     = 0x40,

    /*
        NON-DMA
        
        This mode is selected in the SPECIFY command
        and will be set to a 1 during the execution phase of a command.
        This is for polled data transfers and helps differentiate
        between the data transfer phase and the reading of result bytes.
    */
    BIT_MSR_NDMA    = 0x20,
   #warning To be uzupelnioned

    /*
        Data Register bits (FIFO)

        Each bit can serve different purpose
        in different commands
        and on different steps/phases of command execution
    */

    /*
        Multi-track selector
        
        When set, this flag selects the multi-track operating mode
        In this mode, the 82077AA treats a complete cylinder under head 0 and 1 as a single track
        
        The 82077AA operates as if this expanded track
        started at the first sector under head 0
        and ended at the last sector under head 1
        
        With this flag set, a multitrack read or write operation
        will automatically continue to the first sector under head 1
        when the 82077AA finishes operating on the last sector under head 0
    */
    BIT_MT      = 0x80,

    /*
        MFM mode selector
        
        A one selects the double density (MFM) mode
    */
    BIT_MFM     = 0x40,

    /*
        Skip flag
        
        When set to '1', sectors containing a deleted data address mark
        will automatically be skipped during the execution of READ DATA

        If READ DELETED is executed,
        only sectors with a deleted address mark
        will be accessed
        
        When set to '0', the sector is read or written
        the same as the read and write commands
    */
    BIT_SK      = 0x20,

    /*
        Head address
        
        Selected head: 0 or 1 (disk side 0 or 1) as encoded in the sector ID field
    */
    BIT_HDS     = 0x04,

    /*
        Enable Count
        
        When this bit is '1'
        the 'DTL' parameter of the Verify Command
        becomes 'SC' (Number of sectors per track)
    */
    BIT_EC      = 0x80,

    /*
        Non-DMA mode flag
        
        When set to '1' indicates that the 82077AA is to operate in the non-DMA mode
        In this mode, the host is interrupted for each data transfer

        When set to '0', the 82077AA operates in DMA mode,
        interfacing to a DMA controller by means of the DRQ and DACK signals
    */
    BIT_ND      = 0x01,

    /*
        Enable implied seek
        
        When set, a seek operation will be performed
        before executing any read or write command
        that requires the C parameter in the command phase
        
        A '0' disables the implied seek
    */
    BIT_EIS     = 0x40,

    /*
        Enable FIFO
        
        When this bit is 0, the FIFO is enabled
        A '1' puts the 82077AA in the 8272A compatible mode where the FIFO is disabled
    */
    BIT_EFIFO   = 0x20,

    /*
        Polling disable
        
        When set, the internal polling routine is disabled
        When clear, polling is enabled
    */
    BIT_POLL    = 0x10,

    /*
        Direction control
        
        If this bit is 0, then the head will step out from the spindle during a relative seek
        If set to a 1, the head will step in toward the spindle
    */
    BIT_DIR     = 0x40,

    /*
        Lock defines whether EFIFO, FIFOTHR,
        and PRETRK parameters of the CONFIGURE command
        can be reset to their default values
        by a 'Software Reset'
        (Reset made by setting the proper bit in the DSR or DOR registers)

        DR - Bit used in DUMPREG command
        W - Bit used in LOCK command during Command phase
        R - Bit used in LOCK command during Result phase
    */
    BIT_LOCK_DR = 0x80,
    BIT_LOCK_W  = 0x80,
    BIT_LOCK_R  = 0x10,

    /*
        Drive Select 0-3
        
        Designates which drives are Perpendicular drives,
        a '1' indicating Perpendicular drive
    */
    BIT_D0      = 0x04,
    BIT_D1      = 0x08,
    BIT_D2      = 0x10,
    BIT_D3      = 0x20,

    /*
        Alters Gap 2 length when using Perpendicular Mode
    */
    BIT_GAP     = 0x02,

    /*
        Write gate alters timing of WE
        to allow for pre-erase loads in perpendicular drives
    */
    BIT_WGATE   = 0x01,

    /*
        The bits denoted D0, D1, D2, and D3 of the PERPENDICULAR MODE command
        can only be overwritten when the OW bit is set to '1'
    */
    BIT_OW      = 0x80,
   
    /*
        Digital Input Register bits (DIR)
    */
   #warning To be uzupelnioned
   
    /*
        Configuration Control Register bits (DIR)
    */
   #warning To be uzupelnioned
};

enum fdc_bus_regBitRange {
    /*
        Cylinder address
        
        The currently selected cylinder address, 0 to 255
    */
    RANGE_C     = 0xFF,

    /*
        Head address
        
        Selected head: 0 or 1 (disk side 0 or 1) as encoded in the sector ID field
    */
    RANGE_H     = 0xFF,

    /*
        Sector address
        
        The sector number to be read or written

        In multi-sector transfers,
        this parameter specifies the sector number
        of the first sector to be read or written
    */
    RANGE_R     = 0xFF,

    /*
        Sector size code
        
        This specifies the number of bytes in a sector
        
        If this parameter is '00', then the sector size is 128 bytes
        
        The number of bytes transferred is determined by the DTL parameter
        Otherwise the sector size is 128*(2^N) bytes
        
        All values up to 0x07 are allowable
        
        0x07 would equal a sector size of 16KB
        
        It is the users responsibility to not select combinations
        that are not possible with the drive

        | N    | Size  |
        |------|-------|
        | 0x00 | 128 B |
        | 0x01 | 256 B |
        | 0x02 | 512 B |
        | 0x03 | 1 KB  |
        | 0x04 | 2 KB  |
        | 0x05 | 4 KB  |
        | 0x06 | 8 KB  |
        | 0x07 | 16 KB |
    */
    RANGE_N     = 0xFF,

    /*
        End of track
        
        The final sector number of the current track
    */
    RANGE_EOT   = 0xFF,

    /*
        Gap length
        
        The gap 3 size
        (Gap 3 is the space between sectors excluding the VCO synchronization field)
    */
    RANGE_GPL   = 0xFF,

    /*
        Special sector size
        
        By setting N to zero (00) DTL may be used to control the number of bytes
        transferred in disk read/write commands
        
        The sector size (N == 0) is set to 128
        
        If the actual sector (on the diskette) is larger than DTL:
        * During read commands, the remainder of the actual sector
          is read but is not passed to the host during read commands
        * During write commands, the remainder of the actual sector
          is written with all zero bytes
        
         The CRC check code is calculated with the actual sector
         
         When N is not zero, DTL has no meaning and should be set to 0xFF
    */
    RANGE_DTL       = 0xFF,

    /*
        Number of sectors
        
        * The number of sectors to be initialized by the FORMAT command
        * The number of sectors to be verified during a Verify Command, when EC is set
    */
    RANGE_SC        = 0xFF,

    /*
        DRIVE SELECT - bits 1-0

        The current selected drive
      
        | DS1 | DS0 |    \    |
        |  0  |  0  | drive 0 |
        |  0  |  1  | drive 1 |
        |  1  |  0  | drive 2 |
        |  1  |  1  | drive 3 |
    */
    RANGE_DS        = 0x03,

    /*
        Digital Output Register (DOR)
    */

    /*
        MOTOR CONTROL 4-7

        The MOT ENx bits directly control their respective motor enable pins (ME0 –3).
        A one means the pin is active, the INVERT pin determines the active level.
        The DRIVE SELx bits are decoded to provide four drive select lines and only one may be active at a time. 
        A one is active and the INVERT pin determines the level on the cable. 
        Standard programming practice is to set both MOT ENx and DRIVE SELx bits at the same time.
    */
    RANGE_DOR_MC    = 0xF0,

    /*
        Tape Drive Register sectors (TDR)
    */
   
    /*
        TAPE SELECT 1-0

        Bits 2 thru 7 are not writable and remain tristated if read. 
        The Tape Select bits are Hardware RESET to 0’s, 
        making Drive 0 not available for tape support.
        Drive 0 is ‘‘reserved’’ for the floppy boot drive.
        
        | TS1 | TS2 | DRIVE SELECTED |
        | 0   | 0   | None           |
        | 0   | 1   | 1              |
        | 1   | 0   | 2              |
        | 1   | 1   | 3              |

        The tuning of the PLL for tape characteristics can also be done in hardware.
        If a 0 (GND) is applied to pin 39 (PLL0) 
        the PLL is optimized for tape drives,
        a 1 (VCC) optimizes the PLL for floppies. 
        This hardware selection mechanism overrides the software selection scheme. 
        A typical hardware application would route
        the Drive Select pin used for tape drive support to pin 39 (PLL0).
    */
    RANGE_TDR_TS        = 0x03,

    /*
        Datarate Select Register ranges (DSR)  
    */

    /*
        PRECOMP

        PRECOMP 0–2 adjusts the WRDATA output to the disk
        to compensate for magnetic media phenomena known as bit shifting.
        The data patterns that are susceptible to bit shifting are well understood
        and the 82077AA compensates the data pattern as it is written to the disk.
        The amount of precompensation is dependent upon the drive and media
        but in most cases the default value is acceptable.

        The 82077AA starts precompensating the data pattern starting on Track 0.
        The CONFIGURE command can change the track that precompensating starts on.
        Table 2-2 lists the precompensation values that can be selected
        and Table 2-3 lists the default precompensation values.
        The default value is selected if the three bits are zeros

        Table 2-2. Data Rate Precompensation Delays

        | PRECOMP | Precompensation Delay |
        | 1 1 1   | 0.00 ns - DISABLED    |
        | 0 0 1   | 41.67 ns              |
        | 0 1 0   | 83.34 ns              |
        | 0 1 1   | 125.00 ns             |
        | 1 0 0   | 166.67 ns             |
        | 1 0 1   | 208.33 ns             |
        | 1 1 0   | 250.00 ns             |
        | 0 0 0   | DEFAULT               |

        Table 2-3. Default Precompensation Delays

        | Data rate | Precompensation Delay |
        | 1 Mbps    | 41.67 ns              |
        | 500 Kbps  | 125 ns                |
        | 300 Kbps  | 125 ns                |
        | 250 Kbps  | 125 ns                |
    */
    RANGE_DSR_PRECOMP   = 0x1C,

    /*
        DATA RATE SELECT

        DRATE 0–1 select one of the four data rates as listed in Table 2-4.
        The default value is 250 Kbps upon a chip (‘‘Hardware’’) reset.
        Other (‘‘Software’’) Resets do not affect the DRATE or PRECOMP bits

        Table 2-4. Data Rates
        
        | DRATESEL | Data rate MFM |
        | 1 1      | 1 Mbps        |
        | 0 0      | 500 Kbps      |
        | 0 1      | 300 Kbps      |
        | 1 0      | 250 Kbps      |
    */
    RANGE_DSR_DRATE     = 0x03,

    /*
        Status register 0–3
        
        Registers within the 82077AA
        that store status information after a command has been executed
        
        This status information is available to the host
        during the result phase after command execution
    */
    RANGE_SR0   = 0xFF,
    RANGE_SR1   = 0xFF,
    RANGE_SR2   = 0xFF,
    RANGE_SR3   = 0xFF,

    /*
        Status register 0 subranges
    */

    /*
        Interrupt code - bits 7-6

        Values:
        * 00 - Normal termination of command
        * 01 - Abnormal termination of command - execution started, but not successfully completed
        * 10 - Invalid command
        * 11 - Abnormal termination caused by Polling
    */
    RANGE_SR0_IC    = 0xC0,

    /*
        General
    */

    /*
        Data pattern
        
        The pattern to be written in each sector data field during formatting
    */
    RANGE_D     = 0xFF,

    /*
        
    */
   #warning what the symbol is that
    RANGE_STP   = 0xFF,

    /*
        Present cylinder number
        
        The current position of the head
        at the completion of SENSE INTERRUPT STATUS command
    */
    RANGE_PCN   = 0xFF,

    /*
        Step rate interval
        
        The time interval between step pulses issued by the 82077AA
        
        Programmable from 0.5 to 8 milliseconds,
        in increments of 0.5 ms at the 1 Mbit data rate
        
        Refer to the SPECIFY command for actual delays
    */
    RANGE_SRT   = 0xF0,

    /*
        Head unload time
        
        The time interval from the end of the execution phase
        (of a read or write command) until the head is unloaded
        
        Refer to the SPECIFY command for actual delays
    */
    RANGE_HUT   = 0x0F,

    /*
        Head load time
        
        The time interval that 82077AA waits after loading the head
        and before initiating a read or write operation
        
        Refer to the SPECIFY command for actual delays
    */
    RANGE_HLT   = 0xFE,

    /*
        New cylinder number
        
        The desired cylinder number
    */
    RANGE_NCN   = 0xFF,

    /*
        The FIFO threshold in the execution phase of read or write commands
        
        This is programmable from 1 to 16 bytes
        
        Defaults to one byte
        
        * 0x0 selects one byte
        * 0xF selects 16 bytes
    */
    RANGE_FIFOTHR   = 0x0F,

    /*
        Precompensation start track number
        
        Programmable from track 0x00 to 0xFF
    */
    RANGE_PRETRK    = 0xFF,

    /*
        Relative cylinder number
        
        Relative cylinder offset from present cylinder
        as used by the RELATIVE SEEK command
    */
    RANGE_RCN   = 0xFF,

    /*
        Range marked as 'Undefined'
        in 82077AA Datasheet
    */
    RANGE_Undef = 0xFF
};

/*
    Mask for drives in detected controller
*/
enum driveMask {
    FDD_0   = 0x01,
    FDD_1   = 0x02,
    FDD_2   = 0x04,
    FDD_3   = 0x08
};

/*
    FDD performance configuration

    Contains calibrated HUT, SRT and HLT
    to be stored for each FDD
*/
struct fdc_fddPerf {
    uint_8 hut;
    uint_8 srt;
    uint_8 hlt;
};

/*
    Data rate values

    Used to configure data rate and precompensation delay
    in CCR (layout is the same as in DSR register)

    | Value        | DRATE_SEL1 | DRATE_SEL0 | Data rate | Precompensation delay |
    | RATE_1MBPS   |     1      |     1      | 1 Mbps    |       41.67 ns        |
    | RATE_500KBPS |     0      |     0      | 500 Kbps  |        125 ns         |
    | RATE_300KBPS |     0      |     1      | 300 Kbps  |        125 ns         |
    | RATE_250KBPS |     1      |     0      | 250 Kbps  |        125 ns         |
*/
enum fdc_dataRate {
    RATE_1MBPS      = 0x03,
    RATE_500KBPS    = 0x00,
    RATE_300KBPS    = 0x01,
    RATE_250KBPS    = 0x02
};

/*
    Values for SK bit
*/
enum fdc_skipMode {
    SKIP_ENABLE     = 1,
    SKIP_DISABLE    = 0
};

/*
    Sector size values for N-byte
*/
enum fdc_sectSize {
    SECT_128B       = 0x0,
    SECT_256B       = 0x1,
    SECT_512B       = 0x2,
    SECT_1KB        = 0x3,
    SECT_2KB        = 0x4,
    SECT_4KB        = 0x5,
    SECT_8KB        = 0x6,
    SECT_16KB       = 0x7,
    SECT_UNKNOWN    = 0xFF
};

/*
    Format information for FDD
*/
struct fdc_fddFormat {
    /*
        Gap length value
    */
    uint_8 gpl;

    /*
        Drive format information (system)
    */
    struct gnwStorGeometry sys;
};

/*
    Hardware configuration for FDD
*/
struct fdc_fddConfig {
    /*
        FDC base bus address
    */
    const uint_16 base;

    /*
        FDC drive address
    */
    const uint_8 drive;

    /*
        Multi-track
        
        1 - enabled
        0 - disabled
    */
    const uint_8 mt;

    /*
        Modified frequency modulation encoding used

        1 - enabled (double-density)
        2 - disabled (single-density)
    */
    const uint_8 mfm;

    /*
        FDD performance/reliability configuration
    */
    const struct fdc_fddPerf perf;

    /*
        Drive format information
    */
    const struct fdc_fddFormat format;
};

/*
    Operation status
    
    Status of performing specific command/procedure
    including errors and success 
*/
enum k_fdc_opStatus {
    OPSTATUS_NOT_READY      = -1,
    OPSTATUS_INVPARAM       = -2,
    OPSTATUS_INVCMD         = -3,
    OPSTATUS_INTEXP         = -4,
    OPSTATUS_INVDRIVE       = -5,
    OPSTATUS_MTROFF         = -6,
    OPSTATUS_NOTFIN         = -7,
    OPSTATUS_FAIL           = -8,
    OPSTATUS_ABNORMAL_TERM  = -9,
    OPSTATUS_OK             = 0
};

/*
    Scan command mode

    Each value is associated with command code for given operation
*/
enum fdc_scanMode {
    SM_EQUAL            = 0x11,
    SM_LOW_OR_EQUAL     = 0x19,
    SM_HIGH_OR_EQUAL    = 0x1d
};

/*
    Write command type

    Each value is associated with command code for given operation
*/
enum fdc_writeMode {
    WM_DATA         = 0x05,
    WM_DEL_DATA     = 0x09
};

/*
    Read command type

    Each value is associated with command code for given operation
*/
enum fdc_readMode {
    RM_DATA         = 0x06,
    RM_DEL_DATA     = 0x0c,
    RM_TRACK        = 0x02
};

/*
    Implied seek status
*/
enum fdc_eis {
    ISEEK_ENABLE    = BIT_EIS,
    ISEEK_DISABLE   = 0x00
};

/*
    FIFO status - enabled/disabled
*/
enum fdc_efifo {
    FIFO_ENABLE     = 0x00,
    FIFO_DISABLE    = BIT_EFIFO
};

/*
    Polling mode - enabled/disabled
*/
enum fdc_poll {
    POLL_ENABLE     = 0x00,
    POLL_DISABLE    = BIT_POLL
};

/*
    DMA mode

    The choice of DMA or NON-DMA operations is made by the ND bit
    
    When this bit is '1', the NON-DMA mode is selected,
    and when ND is '0', the DMA mode is selected
    
    In DMA mode, data transfers are signalled by the DRQ pin
    
    Non-DMA mode uses the RQM bit and the INT pin to signal data transfers
*/
enum fdc_dmaMode {
    DMA_ENABLE  = 0x00,
    DMA_DISABLE = BIT_ND
};

/*
    SEEK command mode
*/
enum fdc_seekMode {
    SEEK_ABSOLUTE   = 0x0F,
    SEEK_RELATIVE   = 0x8F
};

#endif // FDC_TYPES_H
