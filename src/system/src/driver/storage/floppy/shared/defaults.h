//
//  defaults.h
//  GunwOS
//
//  Created by Artur Danielewski on 04.04.2020.
//

/*
    Each FDC can support up to 4 drives
*/
#define FDC_FDD_PER_CONTROLLER  4

/*
    The driver supports up to 2 drives
*/
#define FDC_FDD_SUPPORT_COUNT 2

/*
    Value to be the expected result
    of VERSION command
    if the controller is 82077AA
*/
#define FDC_VERSION_82077AA 0x90

/*
    Default delay when waiting for interrupt (in milliseconds)
*/
#define FDC_IRQDELAY_DEFAULT 500

/*
    Amount of retries during wait for command phase
*/
#define FDC_COMMAND_PHASE_RETRY_COUNT   100

/*
    Amount of retries during wait for result phase
*/
#define FDC_RESULT_PHASE_RETRY_COUNT   100

/*
    Amount of milliseconds to wait between phase check retries
*/
#define FDC_PHASE_SLEEPMS_INTERVAL 10

/*
    Default value for Head Unload Time (HUT)

    Used in SPECIFY command
    
    The HUT (Head Unload Time) defines the time from the end
    of the execution phase of one of the read/write commands
    to the head unload state
    
    The value changes with the data rate speed selection
    and is documented in the table below

    HUT (ms):
    | VALUE (col) per DATA RATE (row) | 1Mbps | 500Kbps | 300Kbps | 250Kbps |
    |                              0  | 128   | 256     | 426     | 512     |
    |                              1  |   8   |  16     |  26.7   |  32     |
    |                              .  | ...   | ...     | .....   | ...     |
    |                              E  | 112   | 224     | 373     | 448     |
    |                              F  | 120   | 240     | 400     | 480     |
*/
#define FDC_HUT_DEFAULT 0

/*
    Default value for Step Rate Time (SRT)

    Used in SPECIFY command
    
    The SRT (Step Rate Time) defines the time interval
    between adjacent step pulses

    Note that the spacing between the first and second step pulses
    may be shorter than the remaining step pulses
    
    The value changes with the data rate speed selection
    and is documented in the table below

    SRT (ms):
    | VALUE (col) per DATA RATE (row) | 1Mbps | 500Kbps | 300Kbps | 250Kbps |
    |                              0  |   8.0 |  16     |  26.7   |  32     |
    |                              1  |   7.5 |  15     |  25     |  30     |
    |                              .  |   ... |  ..     |  .....  |  ..     |
    |                              E  |   1.0 |   2     |   3.33  |   4     |
    |                              F  |   0.5 |   1     |   1.67  |   2     |
*/
#define FDC_SRT_DEFAULT 0

/*
    Default value for Head Load Time (HLT)

    Used in SPECIFY command
    
    The HLT (Head Load Time) defines the time
    between the Head Load signal goes high
    and the read/write operation starts
    
    The value changes with the data rate speed selection
    and is documented in the table below

    HLT (ms):
    | VALUE (col) per DATA RATE (row) | 1Mbps | 500Kbps | 300Kbps | 250Kbps |
    |                             00  | 128   | 256     | 426     | 512     |
    |                             01  |   1   |   2     |   3.3   |   4     |
    |                             02  |   2   |   4     |   6.7   |   8     |
    |                             ..  | ..... | ...     | .....   | ...     |
    |                             7E  | 126   | 252     | 420     | 504     |
    |                             7F  | 127   | 254     | 423     | 508     |
*/
#define FDC_HLT_DEFAULT 0

/*
    Default FIFO threshold value

    Equal to 8 bytes (0x00 means 1 byte in FIFOTHR)
*/
#define FDC_FIFOTHR_DEFAULT 7

/*
    Default pre-compensation value
*/
#define FDC_PRECOMP_DEFAULT 0