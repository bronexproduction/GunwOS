//
//  opmode.c
//  GunwOS
//
//  Created by Artur Danielewski on 26.03.2023.
//
//  For more information see:
//  IBM Enhanced Graphics Adapter
//  Section "Modes of operation"
//  page 5+
//

#include "opmode.h"
#include "ega_bus.h"
#include "crt.h"
#include <error/panic.h>

void setMode(const enum modeOfOperation mode) {
    switch(mode) {
        /*
            Alphanumeric Modes

            The data format for alphanumeric modes on the Enhanced Graphics Adapter
            is the same as the data format on the IBM Color/Graphics Monitor Adapter
            and the IBM Monochrome Display Adapter.
            As an added function, bit three of the attribute byte
            may be redefined by the Character Map Select register
            to act as a switch between character sets.
            This gives the programmer access to 512 characters at one time.
            This function is valid only when memory has been expanded to 128K bytes or more.
            When an alphanumeric mode is selected, the BIOS transfers character patterns
            from the ROM to bit plane 2. The processor stores the character data in bit plane 0,
            and the attribute data in bit plane 1. The programmer can view bit planes 0 and 1
            as a single buffer in alphanumeric modes.
            The CRTC generates sequential addresses, and fetches one character code byte
            and one attribute byte at a time. The character code and row scan count
            address bit plane 2, which contains the character generators.
            The appropriate dot patterns are then sent to the palette in the attribute chip,
            where color is assigned according to the attribute data.
        */
    case CD_OPMODE_2: 
    case CD_OPMODE_3: {
    } break;
        /*  
            320x200 Two and Four Color Graphics (Modes 4 and 5)
    
            Addressing, mapping and data format are the same
            as the 320x200 pel mode of the Color/Graphics Monitor Adapter.
            The display buffer is configured at hex B8000.
            Bit image data is stored in bit planes 0 and 1.
        */
    case CD_OPMODE_4:
    case CD_OPMODE_5: {
        #warning stop something before making changes?
        #warning BIOS values from manual
        /*
            External registers
        */
        busWriteExternal(BRE_MISC_OUT, 0x23, mode);
        busWriteExternal(BRE_FEATURE_CTRL, 0x00, mode);

        /*
            Sequencer registers
        */
        busWriteSequencer(BRSI_RESET, 0x03, mode);
        busWriteSequencer(BRSI_CLOCKING_MODE, 0x0B, mode);
        busWriteSequencer(BRSI_MAP_MASK, 0x03, mode);
        busWriteSequencer(BRSI_CHAR_MAP_SELECT, 0x00, mode);
        busWriteSequencer(BRSI_MEMORY_MODE, 0x02, mode);

        /*
            CRT registers
        */
        crtSetMode(mode);

        /*
            Graphics registers
        */
        busWriteGraphics(BRGI_SET_RESET, 0x00, mode);
        busWriteGraphics(BRGI_ENABLE_SET_RESET, 0x00, mode);
        busWriteGraphics(BRGI_COLOR_COMPARE, 0x00, mode);
        busWriteGraphics(BRGI_DATA_ROTATE, 0x00, mode);
        busWriteGraphics(BRGI_READ_MAP_SELECT, 0x00, mode);
        busWriteGraphics(BRGI_MODE_REGISTER, 0x30, mode);
        busWriteGraphics(BRGI_MISC, 0x0F, mode);
        busWriteGraphics(BRGI_COLOR_DONT_CARE, 0x00, mode);
        busWriteGraphics(BRGI_BIT_MASK, 0xFF, mode);

        /*
            Attribute registers
        */
        busWriteAttribute(BRAI_PALETTE_START + 0x0, 0x00, mode);
        busWriteAttribute(BRAI_PALETTE_START + 0x1, 0x13, mode);
        busWriteAttribute(BRAI_PALETTE_START + 0x2, 0x15, mode);
        busWriteAttribute(BRAI_PALETTE_START + 0x3, 0x17, mode);
        busWriteAttribute(BRAI_PALETTE_START + 0x4, 0x02, mode);
        busWriteAttribute(BRAI_PALETTE_START + 0x5, 0x04, mode);
        busWriteAttribute(BRAI_PALETTE_START + 0x6, 0x06, mode);
        busWriteAttribute(BRAI_PALETTE_START + 0x7, 0x07, mode);
        busWriteAttribute(BRAI_PALETTE_START + 0x8, 0x10, mode);
        busWriteAttribute(BRAI_PALETTE_START + 0x9, 0x11, mode);
        busWriteAttribute(BRAI_PALETTE_START + 0xA, 0x12, mode);
        busWriteAttribute(BRAI_PALETTE_START + 0xB, 0x13, mode);
        busWriteAttribute(BRAI_PALETTE_START + 0xC, 0x14, mode);
        busWriteAttribute(BRAI_PALETTE_START + 0xD, 0x15, mode);
        busWriteAttribute(BRAI_PALETTE_START + 0xE, 0x16, mode);
        busWriteAttribute(BRAI_PALETTE_START + 0xF, 0x17, mode);
        busWriteAttribute(BRAI_MODE_CONTROL, BRA_MCR_GRAPHICS_ALPHANUMERIC_MODE, mode);
        busWriteAttribute(BRAI_OVERSCAN_COLOR, 0x00, mode);
        busWriteAttribute(BRAI_COLOR_PLANE_ENABLE, 0x03, mode);
        busWriteAttribute(BRAI_HORIZONTAL_PEL_PANNING, 0x00, mode);

        #warning start something after making changes?
    } break;
    default:
        OOPS("Unsupported display mode");
        return;
    }
}
