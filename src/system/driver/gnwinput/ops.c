//
//  ops.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.02.2025.  
//
//  Information copied from https://wiki.osdev.org/Mouse_Input
//

#include "ops.h"
#include <gunwbus.h>
#include <gunwoutput.h>

#define CAN_READ (rdb(MOUSE_DEVICE_ID, BA_STATUS) & CSR_OUTPUT_BUFFER_FULL)
#define CAN_WRITE (!(rdb(MOUSE_DEVICE_ID, BA_STATUS) & CSR_INPUT_BUFFER_FULL))

extern const size_t MOUSE_DEVICE_ID;

static bool readData(uint_8 * const bytePtr) {
    if (!bytePtr) {
        return false;
    }

    while (!CAN_READ) {
        /*
            Output buffer not ready
        */
        log("Output buffer not ready");
        return false;
    }

    *(bytePtr) = rdb(MOUSE_DEVICE_ID, BA_DATA);
    return true;
}

static bool write(const uint_16 port, const uint_8 byte) {
    if (!CAN_WRITE) {
        /*
            Input buffer not ready
        */
        log("Input buffer not ready");
        return false;
    }

    wrb(MOUSE_DEVICE_ID, port, byte);

    return true;
}

static bool performMouseCommand(const enum mouseCommand command) {
    if (!write(BA_STATUS, KC_PLZ_LEMME_WRITE)) {
        return false;
    }

    if (!write(BA_DATA, command)) {
        return false;
    }
    
    uint_8 result;
    if (!readData(&result)) {
        return false;
    }
    if (result != MCR_ACK) {
        return false;
    }

    return true;
}

static bool performKeyboardCommand(const enum keyboardCommand command) {
    return write(BA_STATUS, command);
}

bool inputEnable() {
    // Set Compaq Status/Enable IRQ12
    // On some systems, the PS2 aux port is disabled at boot. Data coming from the aux port will not generate any interrupts. To know that data has arrived, you need to enable the aux port to generate IRQ12. There is only one way to do that, which involves getting/modifying the "compaq status" byte. You need to send the command byte 0x20 ("Get Compaq Status Byte") to the PS2 controller on port 0x64. If you look at RBIL, it says that this command is Compaq specific, but this is no longer true. This command does not generate a 0xFA ACK byte. The very next byte returned should be the Status byte. (Note: on some versions of Bochs, you will get a second byte, with a value of 0xD8, after sending this command, for some reason.) After you get the Status byte, you need to set bit number 1 (value=2, Enable IRQ12), and clear bit number 5 (value=0x20, Disable Mouse Clock). Then send command byte 0x60 ("Set Compaq Status") to port 0x64, followed by the modified Status byte to port 0x60. This might generate a 0xFA ACK byte from the keyboard.

    return performKeyboardCommand(KC_ENABLE_AUXILIARY_DEVICE);
}

bool reset() {
    if (!performMouseCommand(MC_RESET)) {
        return false;
    }

    uint_8 result;
    if (!readData(&result)) {
        return false;
    }
    if (result != MCR_BAT_SUCCESSFUL) {
        return false;
    }
    if (!readData(&result)) {
        return false;
    }
    if (result != MCR_RESET_COMPLETE) {
        return false;
    }

    return true;
}

bool setDefaults() {
    return performMouseCommand(MC_SET_DEFAULTS);
}

bool enablePacketStreaming() {
    return performMouseCommand(MC_ENABLE_PACKET_STREAMING);
}

bool enableInterrupt() {
    if (!performKeyboardCommand(KC_READ_COMMAND_BYTE)) {
        return false;
    }

    byte_t commandByte;
    #warning command byte value temporarily fixed until drivers refactor to support multiple devices at once
    commandByte = 65;
    // if (!readData(&commandByte)) {
    //     return false;
    // }

    commandByte &= ~CCB_DISABLE_AUXILIARY_DEVICE;
    commandByte |= CCB_ENABLE_AUXILIARY_INTERRUPT;

    if (!performKeyboardCommand(KC_WRITE_COMMAND_BYTE)) {
        return false;
    }

    if (!write(BA_DATA, commandByte)) {
        return false;
    }

    return true;
}

bool readEventData(struct eventDataPacket * const data) {
    if (!data) {
        return false;
    }

    if (!readData((uint_8 *)&(data->state))) {
        return false;
    }
    if (!readData(&(data->movementX))) {
        return false;
    }
    if (!readData(&(data->movementY))) {
        return false;
    }

    return true;
}
