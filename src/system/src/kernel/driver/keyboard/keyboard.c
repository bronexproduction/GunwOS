//
//  keyboard.c
//  GunwOS
//
//  Created by Artur Danielewski on 09.03.2020.
//
//  Docs:
//  https://www.win.tue.nl/~aeb/linux/kbd/scancodes-11.html
//  https://alex.dzyoba.com/blog/os-interrupts/
//  
//

#include <gunwbus.h>
#include <gunwdrv.h>

#include <driver/driver.h>
#include <error/panic.h>

/*
    Keyboard controller data register
*/
#define KBD_BUS_DATA    0x60

/*
    Keyboard controller status register

    MSB                                                 LSB
    | PARE | TIM | AUXB | KEYL | C/D | SYSF | INPB | OUTB |

    PARE - Parity error:

        0 - No error
        1 - Parity error with last byte
    
    TIM - Timeout:

        0 - No timeout
        1 - Timeout:
            * on PS/2 systems: general timeout
            * on AT systems: timeout on transmission from keyboard to keyboard controller, possible parity error (check parity bit)
    
    AUXB - Auxiliary output buffer full:
    
        PS/2 systems: if OUTB == 1 then read from 0x60 will be valid and AUXB tells if data comes from keyboard (0) or mouse (1)
        AT systems:
            0 - OK
            1 - Timeout on transmission from keyboard controller to keyboard (this may indicate that no keyboard is present)

    KEYL - Keyboard lock:

        0 - Locked
        1 - Not locked
    
    C/D - Command/Data:

        NOTE: This bit is also referred to as Address Line A2

        0 - Last write to input buffer was data (via port 0x60)
        1 - Last write to input buffer was a command (via port 0x64)

    SYSF - System flag:

        0 - State after power on / reset
        1 - State after successful completion of the keyboard controller self-test (Basic Assurance Test, BAT); can also be set by command
    
    INPB - Input buffer status:

        0 - Input buffer empty, can be written
        1 - Input buffer full, don't write yet
    
    OUTB - Output buffer status:

        0 - Output buffer empty, don't read yet
        1 - Output buffer full, can be read

        NOTE: This bit is cleared when port 0x60 is read
*/
#define KBD_BUS_STATUS      0x64

#define KBD_STAT_OUTB       0x01    /* Output buffer status bit (OUTB) */
#define KBD_STAT_INPB       0x02    /* Input buffer status bit (INPB) */
#define KBD_STAT_SYSF       0x04    /* System flag bit (SYSF) */
#define KBD_STAT_CMD_DAT    0x08    /* Command/data bit (C/D) */
#define KBD_STAT_KEYL       0x10    /* Keyboard lock bit (KEYL) */
#define KBD_STAT_AUXBF      0x20    /* Auxiliary output buffer full bit (AUXB) */
#define KBD_STAT_TIM        0x40    /* Timeout bit (TIM) */
#define KBD_STAT_PARERR     0x80    /* Parity error bit (PARE) */

// struct listener {
//     gnwSysCallback_u8 keyUp;
//     gnwSysCallback_u8 keyDown;
// };

ISR(
    /* Checking output buffer status */
    if (!rdb(KBD_BUS_STATUS) & KBD_STAT_OUTB) {
        OOPS("Keyboard output buffer empty on keyboard interrupt")
        ISR_END
    }

    /* Reading keycode */
    uint_8 c = rdb(KBD_BUS_DATA);
    
    /*
        Extracting exact keycode

        MSB contains information whether key was pressed or released
    */
    if (c & 0b10000000) {
        // user_cli_kbf_up(c & 0b01111111);
    }
    else {
        // user_cli_kbf_down(c);
    }
)

static struct gnwDriverConfig desc() {
    return (struct gnwDriverConfig){ 0, 0, isr, 1 };
}

static struct gnwDeviceUHA uha() {
    struct gnwDeviceUHA uha;

    uha.event.desc.eventDataFormat = GDEF_U8;

    return uha;
}

struct gnwDeviceDescriptor k_drv_keyboard_descriptor() {
    return (struct gnwDeviceDescriptor) {
        /* type */ DEV_TYPE_KEYBOARD,
        /* api */ uha(),
        /* driver */ (struct gnwDeviceDriver) {
            /* io */ (struct gnwDeviceIO) {
                /* busBase */ 0x60
            },
            /* descriptor */ desc()
        },
        /* name */ "8042 PS/2 Controller"
    };
}
