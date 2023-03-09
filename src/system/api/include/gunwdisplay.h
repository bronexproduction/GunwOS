//
//  gunwdisplay.h
//  GunwOS
//
//  Created by Artur Danielewski on 17.01.2023.
//

#ifndef GUNWOS_GUNWDISPLAY_H
#define GUNWOS_GUNWDISPLAY_H

#include <stdgunw/types.h>
#include <uha/gunwuha_display_desc.h>
#include <gunwdev.h>

#warning may not work from DPL_3 yet

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
        Invalidate and clear the handle
    */
    void (*invalidate)(struct gnwTextDisplayHandle * const handle);

    /*
        Update the framebuffer (text mode)
    
        Params:
            * Framebuffer data (X * Y * sizeof(struct gnwDeviceUHA_display_character))
    */
    enum gnwDeviceError (*update)(struct gnwTextDisplayHandle * const handle,
                                  const struct gnwDeviceUHA_display_character * const buffer);
};

struct gnwGraphicsDisplayHandle {
    /*
        Display descriptor
    */
    struct gnwDisplayDescriptor descriptor;

    /*
        Invalidate and clear the handle
    */
    void (*invalidate)(struct gnwGraphicsDisplayHandle * const handle);

    /*
        Update the framebuffer (graphics mode)
    
        Params:
            * Framebuffer data (X * Y * sizeof(struct gnwDeviceUHA_display_pixel))
    */
    enum gnwDeviceError (*update)(struct gnwGraphicsDisplayHandle * const handle,
                                  const struct gnwDeviceUHA_display_pixel * const buffer);
};

/*
    Functions for retrieving display info

    Result:
        * Display descriptor for the default display of given type (text/graphics)
*/
enum gnwDeviceError getTextDisplay(struct gnwDisplayDescriptor * const displayDescriptor);
enum gnwDeviceError getGraphicsDisplay(struct gnwDisplayDescriptor * const displayDescriptor);

/*
    Functions for attaching to the display

    Params:
        * displayId - Display identifier, found in struct gnwDisplayDescriptor - see getTextDisplay/getGraphicsDisplay
        * handle - Display handle location (mandatory) - filled with handle information on success

    Result:
        * Error value indicating what went wrong while attaching

    Note: Only the process attached to the display can access it
*/
enum gnwDeviceError attachToTextDisplay(uint_32 displayId, struct gnwTextDisplayHandle * const handle);
enum gnwDeviceError attachToGraphicsDisplay(uint_32 displayId, struct gnwGraphicsDisplayHandle * const handle);

#endif // GUNWOS_GUNWDISPLAY_H
