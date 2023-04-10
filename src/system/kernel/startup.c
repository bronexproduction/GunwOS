//
//  startup.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.03.2023.
//

#include <gunwctrl.h>
#include <error/panic.h>

void k_startup() {
    enum gnwCtrlError err = start("cli");
    if (err != GCE_NONE) {
        OOPS("Unable to start core modules");
    }
}
