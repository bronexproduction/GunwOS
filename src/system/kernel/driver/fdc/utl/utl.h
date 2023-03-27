//
//  utl.h
//  GunwOS
//
//  Created by Artur Danielewski on 26.03.2023.
//

#ifndef UTL_H
#define UTL_H

#include "../common/fdctypes.h"

struct fdc_fddConfig configFor(const uint_32 driveID);
enum fdc_sectSize sectSizeForBytes(const uint_16 sectBytes);
size_t bytesForSectSize(const enum fdc_sectSize sectSize);
struct gnwStorGeometry driveGeometry(const uint_8 index);

/*
    Wait for interrupt

    Returns 1 if the interrupt flag gets set, 0 otherwise

    NOTE: irqRecv flag should be cleared prior to waitForInterrupt() call
*/
uint_8 waitForInterrupt(const time_t ms);

/*
    Indicates data being ready to read from FIFO in non-DMA mode
*/
bool isReadyForNonDMARead(const uint_16 base);

/*
    Indicates execution phase in non-DMA mode

    This mode is selected in the SPECIFY command
    and will be set to a 1 during the execution phase of a command. 
    This is for polled data transfers
    and helps differentiate between the data transfer phase
    and the reading of result bytes
*/
bool inNonDMAExecutionPhase(const uint_16 base);

/*
    Checks if the controller is in polling (non-DMA) mode
*/
bool inPollingMode(const uint_16 base);

bool inCommandPhase(const uint_16 base);
bool inResultPhase(const uint_16 base);

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
bool waitForCommandPhase(const uint_16 base);

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
bool waitForResultPhase(const uint_16 base);

#endif // UTL_H
