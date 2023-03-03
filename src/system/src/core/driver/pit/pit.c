//
//  pit.c
//  GunwOS
//
//  Created by Artur Danielewski on 19.03.2020.
//
//  Docs:
//  https://wiki.osdev.org/Programmable_Interval_Timer
//

#include <gunwbus.h>
#include <gunwdrv.h>

#include "../driver.h"
#include "../../kernel/hal/io/io.h"
#include "../../log/log.h"

#define PIT_BUS_DATA_CH0    0x40    // Channel 0 (read/write)
#define PIT_BUS_DATA_CH1    0x41    // Channel 1 (read/write)
#define PIT_BUS_DATA_CH2    0x42    // Channel 2 (read/write)
#define PIT_BUS_COMMAND     0x43    // Mode/Command register (write only)

/*
    Command register

    MSB                                                                                                                                                 LSB
    |               7 - 6                 |               5 - 4              |                          3 - 1                        |         0          |
    | Channel select bits                 | Access mode                      | Operating mode:                                       | BCD/Binary mode    |
    | 0 0 - channel 0                     | 0 0 - latch count value command  | 0 0 0 = Mode 0 (interrupt on terminal count)          | 0 - 16-bit binary  |
    | 0 1 - channel 1                     | 0 1 - access mode: lobyte only   | 0 0 1 = Mode 1 (hardware re-triggerable one-shot)     | 1 - four-digit BCD |
    | 1 0 - channel 2                     | 1 0 - access mode: hibyte only   | 0 1 0 = Mode 2 (rate generator)                       |                    |
    | 1 1 - read-back command (8254 only) | 1 1 - access mode: lobyte/hibyte | 0 1 1 = Mode 3 (square wave generator)                |                    |
    |                                     |                                  | 1 0 0 = Mode 4 (software triggered strobe)            |                    |
    |                                     |                                  | 1 0 1 = Mode 5 (hardware triggered strobe)            |                    |
    |                                     |                                  | 1 1 0 = Mode 2 (rate generator, same as 0 1 0)        |                    |
    |                                     |                                  | 1 1 1 = Mode 3 (square wave generator, same as 0 1 1) |                    |

    Channel 0 is connected to PIC chip and generates IRQ 0. It's the only channel we currently use
    Channel 1 is probably deprecated and had been used as DRAM refresh trigger
    Channel 2 is responsible for PC speaker - may use it in future
*/

/*
    Channel select bits
*/
#define PIT_CMD_CSB_CH0     0x00
#define PIT_CMD_CSB_CH1     0x40
#define PIT_CMD_CSB_CH2     0x80
#define PIT_CMD_CSB_RBC     0xc0

/*
    Access mode bits
*/
#define PIT_CMD_ACC_LCOUNT  0x00
#define PIT_CMD_ACC_LO      0x10
#define PIT_CMD_ACC_HI      0x20
#define PIT_CMD_ACC_LOHI    0x30

/*
    Operating mode bits
*/
#define PIT_CMD_MOD_0       0x00
#define PIT_CMD_MOD_1       0x02
#define PIT_CMD_MOD_2       0x04
#define PIT_CMD_MOD_3       0x06
#define PIT_CMD_MOD_4       0x08
#define PIT_CMD_MOD_5       0x0a

/*
    Countdown register format
*/
#define PIT_CMD_CRF_BINARY  0x00
#define PIT_CMD_CRF_BCD     0x01

/*
    PIT working frequency in Hz
*/
const uint_32 k_pit_freq = 1193182;

/*
    Expected PIT event frequency in Hz
*/
uint_16 k_pit_evFreq;

/*
    Handler for PIT interrupt
*/
void (*k_pit_routine)();

static uint_16 divider() {
    return k_pit_freq / k_pit_evFreq;
}

static uint_8 init() {

    if (!k_pit_routine) {
        LOG_FATAL("PIT routine unavailable")
        return 0;
    }
    
    uint_16 div = divider();
    wrb(PIT_BUS_COMMAND, PIT_CMD_CSB_CH0 | PIT_CMD_ACC_LOHI | PIT_CMD_MOD_3 | PIT_CMD_CRF_BINARY);
    k_io_wait();
    wrb(PIT_BUS_DATA_CH0, div & 0xFF);
    k_io_wait();
    wrb(PIT_BUS_DATA_CH0, div >> 8);
    k_io_wait();

    return 1;
}

ISR(
    k_pit_routine();
)

static struct gnwDriverConfig desc() {
    return (struct gnwDriverConfig){ init, 0, isr, 0 };
}

static struct gnwDeviceUHA uha() {
    struct gnwDeviceUHA uha;

    uha.system.desc._unused = 0;

    return uha;
}

struct gnwDeviceDescriptor c_drv_pit_descriptor() {
    return (struct gnwDeviceDescriptor) {
        DEV_TYPE_SYSTEM,
        uha(),
        (struct gnwDeviceDriver) {
            (struct gnwDeviceIO) {
                0x40
            },
            desc()
        },
        "8253/8254 Programmable Interrupt Timer"
    };
}
