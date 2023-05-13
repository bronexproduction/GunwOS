//
//  startup.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.03.2023.
//

#include <src/_gunwctrl.h>
#include <hal/proc/proc.h>
#include <error/panic.h>

void k_startup() {
    
    extern void k_scr_usr_start(const procId_t, const struct gnwCtrlStartDescriptor * const);
    enum gnwCtrlError err;
    /*
        Load terminal
    */
    {
        struct gnwCtrlStartDescriptor desc;
        desc.pathPtr = "0:GUNWTERM.ELF";
        desc.pathLen = 14;
        desc.errorPtr = &err;
        k_scr_usr_start(KERNEL_PROC_ID, &desc);
    }

    /*
        Load command line
    */
    if (!err) {
        struct gnwCtrlStartDescriptor desc;
        desc.pathPtr = "0:GUNWSH.ELF";
        desc.pathLen = 12;
        desc.errorPtr = &err;
        k_scr_usr_start(KERNEL_PROC_ID, &desc);
    }
    if (err != GCE_NONE) {
        OOPS("Unable to start core modules");
    }
}
