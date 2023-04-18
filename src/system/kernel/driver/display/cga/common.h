//
//  common.h
//  GunwOS
//
//  Created by Artur Danielewski on 18.04.2023.
//

#ifndef COMMON_H
#define COMMON_H

static const volatile ptr_t CGA_VIDEO_HW_MEM = (volatile ptr_t)0xb8000;

#define CGA_BUS_MODE_CONTROL_REG    0x3D8
#define CGA_BUS_COLOR_CONTROL_REG   0x3D9

enum k_drv_display_cga_modeRegBit {
    /*
        Bit 5: 1 to enable blinking, 0 to disable it
        
        In text modes, if bit 5 is 1, characters with attribute bit 7 set will blink.
        If not, they will have high intensity background.
        This has no effect in graphics modes.
    */
    CGA_MRB_BLINK       = 0x20,

    /*
        Bit 4: High-resolution graphics
    
        If this bit is set, it selects 2-colour graphics (640 pixels wide)
        rather than 4-colour (320 pixel wide).
        
        In text mode, setting this bit has the following effects:
            * The border is always black.
            * The characters displayed are missing columns
              as if the bit pattern has been ANDed with another value.
              According to reenigne.org, the value is the equivalent bit pattern
              from the 640x200 graphics mode.
    */
    CGA_MRB_HI_RES      = 0x10,

    /*
        Bit 3: Enable video output
    
        If bit 3 is 0, screen output will not be shown
        (it will be exactly as if the video RAM contained all zeroes).
        
        The usual use of this is if you're reprogramming the CRTC registers;
        disable video output beforehand and re-enable it after.
    */
    CGA_MRB_VOUT_ENABLE = 0x08,

    /*
        Bit 2: Black and white
    
        If the card is displaying on a composite monitor,
        this disables the NTSC color(sic) burst, giving black and white output.
        
        On an RGB monitor it has no effect except in the 320x200 graphics mode,
        when it selects a third palette (black/red/cyan/white).
        This palette is not documented, and not all of IBM's later CGA-compatible cards support it.
        If this bit is set to zero in 640x200 mode, you get colour composite mode.
    */
    CGA_MRB_MONOCHROME  = 0x04,

    /*
        Bit 1: Graphics mode
        
        If this bit is set, the display RAM will be treated as bitmap graphics rather than as text.
    */
    CGA_MRB_GRAPHICS    = 0x02,

    /*
        Bit 0: High resolution
        
        This bit should only be set in the 80-column text mode.
        
        It changes various timing parameters (to display 160 bytes per row instead of 80);
        the CRTC will need to be reprogrammed accordingly.
    */
    CGA_MRB_HI_RES_TEXT = 0x01,
};

#endif // COMMON_H