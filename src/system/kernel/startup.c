//
//  startup.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.03.2023.
//

#include <gunwctrl.h>
#include <error/panic.h>

void k_startup() {
    extern enum gnwCtrlError k_scr_usr_start(const char * const, const size_t);
    enum gnwCtrlError err = k_scr_usr_start("0:/gunwsh.elf", 13);
    if (err != GCE_NONE) {
        OOPS("Unable to start core modules");
    }
}
