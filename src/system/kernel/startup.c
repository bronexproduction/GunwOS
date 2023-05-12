//
//  startup.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.03.2023.
//

#include <gunwctrl.h>
#include <hal/proc/proc.h>
#include <error/panic.h>

void k_startup() {
    
    extern void k_scr_usr_start(const procId_t, const char * const, const size_t, enum gnwCtrlError * const);
    enum gnwCtrlError err;
    /*
        Load terminal
    */
    k_scr_usr_start(KERNEL_PROC_ID, "0:GUNWTERM.ELF", 14, &err);

    /*
        Load command line
    */
    if (!err) {
        k_scr_usr_start(KERNEL_PROC_ID, "0:GUNWSH.ELF", 12, &err);
    }
    if (err != GCE_NONE) {
        OOPS("Unable to start core modules");
    }
}
