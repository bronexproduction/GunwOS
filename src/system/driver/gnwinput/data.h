//
//  data.h
//  GunwOS
//
//  Created by Artur Danielewski on 11.02.2025.
//
//  Information copied from https://wiki.osdev.org/Mouse_Input
//  See more in "Keyboard ans Auxiliary Device Controller" IBM documentation
//

#ifndef DATA_H
#define DATA_H

#include <types.h>

#define IRQ_KEYBOARD 1
#define IRQ_MOUSE 12

#define KEYBOARD_DEVICE_ID (_gnw_device_identifier_list[0])
#define MOUSE_DEVICE_ID (_gnw_device_identifier_list[1])

extern const size_t _gnw_device_identifier_list[];

enum busAddress {
    /*
        Controller status register bus address
    */
    BA_STATUS = 0x64,
    
    /*
        Controller data register bus address (mouse on auxiliary input)
    */
    BA_DATA = 0x60
};

/*
    Mouse commands
*/
enum mouseCommand {
    /*
        Reset

        The mouse probably sends ACK (0xFA) plus several more bytes, then resets itself, and always sends 0xAA.
    */
    MC_RESET = 0xFF,
    
    /*
        Resend

        This command makes the mouse send its most recent packet to the host again.
    */
    MC_RESEND = 0xFE,
    
    /*
        Set Defaults

        Disables streaming, sets the packet rate to 100 per second, and resolution to 4 pixels per mm.
    */
    MC_SET_DEFAULTS = 0xF6,

    /*
        Disable Packet Streaming
    
        The mouse stops sending automatic packets.
    */
    MC_DISABLE_PACKET_STREAMING = 0xF5,

    /*
        Enable Packet Streaming

        The mouse starts sending automatic packets when the mouse moves or is clicked.
    */
    MC_ENABLE_PACKET_STREAMING = 0xF4,

    /*
        Set Sample Rate

        Set Sample Rate	Requires an additional data byte: automatic packets per second (see below for legal values).
    */
    MC_SET_SAMPLE_RATE = 0xF3,
    
    /*
        Get Mouse ID

        The mouse sends sends its current "ID", which may change with mouse initialization.
    */
    MC_GET_MOUSE_ID = 0xF2,
    
    /*
        Request Single Packet

        The mouse sends ACK, followed by a complete mouse packet with current data.
    */
    MC_REQUEST_SINGLE_PACKET = 0xEB,

    /*
        Status Request

        The mouse sends ACK, then 3 status bytes. See below for the status byte format.
    */
    MC_STATUS_REQUEST = 0xE9,
    
    /*
        Set Resolution
        
        Requires an additional data byte: pixels per millimeter resolution (value 0 to 3)
    */
    MC_SET_RESOLUTION = 0xE8,
    
    /*
        Set Remote Mode

        The mouse sends ACK (0xFA) and then reset its movement counters, and enters remote mode
    */
    MC_SET_REMOTE_MODE = 0xF0,

    /*
        Set Wrap Mode

        The mouse sends ACK (0xFA) and then reset its movement counters, and enters wrap mode
    */
    MC_SET_WRAP_MODE = 0xEE,
    
    /*
        Reset Wrap Mode

        The mouse sends ACK, and then enters the last mode, before entering wrap mode, it also resets its movement counters
    */
    MC_RESET_WRAP_MODE = 0xEC,

    /*
        Set Stream Mode

        The mouse sends ACK (0xFA) and then reset its movement counters, and enters reporting mode
    */
    MC_SET_STREAM_MODE = 0xEA,

    /*
        Set Scaling 2:1

        The mouse sends ACK and sets non-linear scaling "2:1"
    */
    MC_SET_SCALING_2_1 = 0xE7,

    /*
        Set Scaling 1:1

        The mouse sends ACK and sets normal linear scaling "1:1"
    */
    MC_SET_SCALING_1_1 = 0xE6
};

/*
    Keyboard commands
*/
enum keyboardCommand {
    /*
        Read controller command byte
    */
    KC_READ_COMMAND_BYTE = 0x20,
    
    /*
        Write controller command byte
    */
    KC_WRITE_COMMAND_BYTE = 0x60,

    /*
        Enable Auxiliary Device
    */
    KC_ENABLE_AUXILIARY_DEVICE = 0xA8,
    
    /*
        Please let me write data to mouse
    */
    KC_PLZ_LEMME_WRITE = 0xD4
};

/*
    Controller Command Byte bits
*/
enum controllerCommandByte {
    /*
        Setting this bit to 1 causes the controller to generate an
        interrupt (IRQ 1) when it places keyboard or command
        controller response data into its output buffer.
    */
    CCB_ENABLE_KEYBOARD_INTERRUPT = 0x01,

    /*
        Setting this bit to 1 causes the controller to generate an
        interrupt (IRQ 12) when it places auxiliary device data into
        its output buffer.
    */
    CCB_ENABLE_AUXILIARY_INTERRUPT = 0x02,

    /*
        The value written to this bit is placed in the system flag bit
        of the Controller Status register.
    */
    CCB_SYSTEM_FLAG = 0x04,

    /*
        This bit is reserved.
    */
    CCB_RESERVED_1 = 0x08,

    /*
        Setting this bit to 1 disables the keyboard interface by
        driving the ‘clock’ line low. Data is not received while the
        interface is disabled.
    */
    CCB_DISABLE_KEYBOARD = 0x10,

    /*
        Setting this bit to 1 disables the auxiliary device interface
        by driving the ‘clock’ line low. Data is not received while
        the interface is disabled.
    */
    CCB_DISABLE_AUXILIARY_DEVICE = 0x20,

    /*
        When this bit is set to 1,
        the Type 1 controller translates the incoming keyboard scan codes
        to scan set 1. When this bit is set to 0,
        the controller passes the incoming scan codes
        without translation. Following power-on or a keyboard reset,
        the keyboard transmits using scan code set 2.
        
        On the Type 2 controller, this bit cannot be set to 1:
        therefore, it can be used to determine the type of controller.
        Writing this bit as a 1 and reading it as a0
        indicates a Type 2 controller.
        
        For keyboard operations
        that are compatible with IBM Personal Computers,
        the Type 1 controller is placed in the translate mode.
        To perform the same operations with the Type 2 controller,
        the keyboard is set up to transmit in scan code set 1
        by using the Select Alternate Scan Codes command
        (see the Keyboard section for more information).
        
        Note: For Type 1 controllers, this bit must be set to 0
        while requesting the keyboard for its scan set.
        This prevents the controller from translating the keyboard response.
    */
    CCB_KEYBOARD_TRANSLATE = 0x40,

    /*
        This bit is reserved.
    */
    CCB_RESERVED_2 = 0x80
};

/*
    Controller Status Register bits
*/
enum controllerStatusRegister {
    /*
        PARE - Parity error:

        0 - No error
        1 - Parity error with last byte
    */
    CSR_PARITY_ERROR = 0x80,

    /*
        TIM - Timeout:

        0 - No timeout
        1 - Timeout:
            * on PS/2 systems: general timeout
            * on AT systems: timeout on transmission from keyboard to keyboard controller, possible parity error (check parity bit)
    */
    CSR_TIMEOUT = 0x40,

    /*
        AUXB - Auxiliary output buffer full:
    
        PS/2 systems: if OUTB == 1 then read from 0x60 will be valid and AUXB tells if data comes from keyboard (0) or mouse (1)
        AT systems:
            0 - OK
            1 - Timeout on transmission from keyboard controller to keyboard (this may indicate that no keyboard is present)
    */
    CSR_AUXILIARY_OUTPUT_BUFFER_FULL = 0x20,

    /*
        KEYL - Keyboard lock:

        0 - Locked
        1 - Not locked
    */
    CSR_KEYBOARD_LOCK = 0x10,

    /*
        C/D - Command/Data:

        NOTE: This bit is also referred to as Address Line A2

        0 - Last write to input buffer was data (via port 0x60)
        1 - Last write to input buffer was a command (via port 0x64)
    */
    CSR_COMMAND_FLAG = 0x08,

    /*
        SYSF - System flag:

        0 - State after power on / reset
        1 - State after successful completion of the keyboard controller self-test (Basic Assurance Test, BAT); can also be set by command
    */
    CSR_SYSTEM_FLAG = 0x04,

    /*
        INPB - Input buffer status:

        0 - Input buffer empty, can be written
        1 - Input buffer full, don't write yet
    */
    CSR_INPUT_BUFFER_FULL = 0x02,
    
    /*
        OUTB - Output buffer status:

        0 - Output buffer empty, don't read yet
        1 - Output buffer full, can be read

        NOTE: This bit is cleared when port 0x60 is read
    */
    CSR_OUTPUT_BUFFER_FULL = 0x01
};

/*
    Mouse command response codes
*/
enum mouseCommandResponse {
    MCR_BAT_SUCCESSFUL = 0xAA,
    MCR_ACK = 0xFA,
    MCR_RESET_COMPLETE = 0x00
};

/*
    Movement data packet
*/
__attribute__((packed)) struct eventDataPacket {
    struct {
        bool overflowY    :1;
        bool overflowX    :1;
        bool signBitY     :1;
        bool signBitX     :1;
        bool _unused      :1;
        bool buttonMiddle :1;
        bool buttonRight  :1;
        bool buttonLeft   :1;
    } state;
    byte_t movementX;
    byte_t movementY;
};

#endif // DATA_H
