//
//  cmd_base.c
//  GunwOS
//
//  Created by Artur Danielewski on 27.03.2020.
//
//  Docs:
//  Intel 82077AA Datasheet 
//

#include "../shared/io.h"
#include "../shared/defaults.h"
#include "../../../../../include/gunwscl.h"

static uint_8 inCommandPhase(const uint_16 base);
static uint_8 inResultPhase(const uint_16 base);

/*
    Waiting for command phase

    Before writing to the 82077AA, the host must examine
    the RQM and DIO bits of the Main Status Register
    
    RQM, DIO must be equal to '1' and '0' respectively
    before command bytes may be written 
    
    RQM is set false by the 82077AA after each write cycle
    until the received byte is processed
    
    The 82077AA asserts RQM again to request each parameter byte of the command,
    unless an illegal command condition is detected
    
    After the last parameter byte is received, RQM remains '0',
    and the 82077AA automatically enters the next phase
    as defined by the command definition

    The FIFO is disabled during the command phase
    
    to retain compatibility with the 8272A,
    and to provide for the proper handling
    of the 'Invalid Command' condition

*/
uint_8 fdc_waitForCommandPhase(const uint_16 base) {
    for (uint_32 i = 0; i < FDC_COMMAND_PHASE_RETRY_COUNT; ++i) {
        if (inCommandPhase(base)) {
            return 1;
        }

        sleepms(FDC_PHASE_SLEEPMS_INTERVAL);
    }

    return 0;
}

/*
    Waiting for result phase

    The generation of INT determines the beginning of the result phase
    
    For each of the commands,
    a defined set of result bytes has to be read from the 82077AA
    before the result phase is complete
    (Refer to Section 5.0 on command descriptions)
    
    These bytes of data must be read out for another command to start

    RQM and DIO must both equal '1'
    before the result bytes may be read from the FIFO
    
    After all the result bytes have been read,
    the RQM and DIO bits switch to '1' and '0' respectively,
    and the CB bit is cleared.
    This indicates that the 82077AA is ready to accept the next command.

*/
uint_8 fdc_waitForResultPhase(const uint_16 base) {
    for (uint_32 i = 0; i < FDC_RESULT_PHASE_RETRY_COUNT; ++i) {
        if (inResultPhase(base)) {
            return 1;
        }

        sleepms(FDC_PHASE_SLEEPMS_INTERVAL);
    }

    return 0;
}

/*
    Indicates execution phase in non-DMA mode

    This mode is selected in the SPECIFY command
    and will be set to a 1 during the execution phase of a command. 
    This is for polled data transfers
    and helps differentiate between the data transfer phase
    and the reading of result bytes
*/
uint_8 fdc_inNonDMAExecutionPhase(const uint_16 base) {
    uint_8 msr = rdb(base + REG_MSR);
    


    return IS_SET(BIT_MSR_NDMA, msr);
}

/*
    Indicates data being ready to read from FIFO in non-DMA mode
*/
uint_8 fdc_readyForNonDMARead(const uint_16 base) {
    uint_8 msr = rdb(base + REG_MSR);
    
    return IS_SET(BIT_MSR_RQM, msr) && IS_SET(BIT_MSR_DIO, msr) && IS_SET(BIT_MSR_NDMA, msr);
}

static uint_8 inCommandPhase(const uint_16 base) {
    uint_8 msr = rdb(base + REG_MSR);

    return IS_SET(BIT_MSR_RQM, msr) && IS_NOT_SET(BIT_MSR_DIO, msr);
}

static uint_8 inResultPhase(const uint_16 base) {
    uint_8 msr = rdb(base + REG_MSR);
    
    return IS_SET(BIT_MSR_RQM, msr) && IS_SET(BIT_MSR_DIO, msr);
}
