//
//  gunwdisplay.c
//  GunwOS
//
//  Created by Artur Danielewski on 17.01.2023.
//

#include <gunwdisplay.h>

enum displayType {
    TEXT,
    GRAPHICS
};

static struct gnwDisplayDescriptor getDisplay(enum displayType type) {

}

struct gnwDisplayDescriptor getTextDisplay() {
    return getDisplay(TEXT);
}

struct gnwDisplayDescriptor getGraphicsDisplay() {
    return getDisplay(GRAPHICS);
}

struct gnwTextDisplayHandle attachToTextDisplay(uint_32 displayId, enum gnwDisplayError *e) {

}

struct gnwGraphicsDisplayHandle attachToGraphicsDisplay(uint_32 displayId, enum gnwDisplayError *e) {

}
 