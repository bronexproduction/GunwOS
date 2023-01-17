//
//  gunwdisplay.h
//  GunwOS
//
//  Created by Artur Danielewski on 17.01.2023.
//

#ifndef GUNWOS_GUNWDISPLAY_H
#define GUNWOS_GUNWDISPLAY_H

#include <stdgunw/types.h>
#include <uha/gunwuha_display.h>

enum gnwDisplayError {
    NONE = 0,
    HANDLE_INVALID
};

struct gnwDisplayDescriptor { 
    /*
        Display identifier
    */
    uint_32 identifier;
    
    /*
        Display resolution
    */
    point_t dimensions;

    /*
        Display format
    */
    enum gnwDeviceUHA_display_format format;
};

struct gnwTextDisplayHandle {
    /*
        Display descriptor
    */
    struct gnwDisplayDescriptor descriptor;

    /*
        Update the framebuffer (text mode)
    
        Params:
            * Framebuffer data (X * Y * sizeof(struct gnwDeviceUHA_display_character))
    */
    enum gnwDisplayError (*update)(const struct gnwDeviceUHA_display_character * const buffer);
};

struct gnwGraphicsDisplayHandle {
    /*
        Display descriptor
    */
    struct gnwDisplayDescriptor descriptor;

    /*
        Update the framebuffer (graphics mode)
    
        Params:
            * Framebuffer data (X * Y * sizeof(struct gnwDeviceUHA_display_pixel))
    */
    enum gnwDisplayError (*update)(const struct gnwDeviceUHA_display_pixel * const buffer);
};

/*
    Functions for retrieving display info

    Result:
        * Display descriptor for the default display of given type (text/graphics)
*/
struct gnwDisplayDescriptor getTextDisplay();
struct gnwDisplayDescriptor getGraphicsDisplay();

/*
    Functions for attaching to the display

    Params:
        * displayId - Display identifier, found in struct gnwDisplayDescriptor - see getTextDisplay/getGraphicsDisplay
        * e - Error return pointer (mandatory) - stores an error value indicating what went wrong while attaching

    Result:
        * Display handle for attached screen

    Note: Only the process attached to the display can access it
*/
struct gnwTextDisplayHandle attachToTextDisplay(uint_32 displayId, enum gnwDisplayError *e);
struct gnwGraphicsDisplayHandle attachToGraphicsDisplay(uint_32 displayId, enum gnwDisplayError *e);

#endif // GUNWOS_GUNWDISPLAY_H
