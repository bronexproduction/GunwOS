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
        Load display manager
    */
    {
        struct gnwCtrlStartDescriptor desc;
        desc.pathPtr = "0:GNWDISPM.ELF";
        desc.pathLen = 14;
        desc.errorPtr = &err;
        k_scr_usr_start(KERNEL_PROC_ID, &desc);
    }

    /*
        Load terminal
    */
    if (err == GCE_NONE) {
        struct gnwCtrlStartDescriptor desc;
        desc.pathPtr = "0:GNWTERM.ELF";
        desc.pathLen = 13;
        desc.errorPtr = &err;
        k_scr_usr_start(KERNEL_PROC_ID, &desc);
    }

    /*
        Load command line
    */
    if (err == GCE_NONE) {
        struct gnwCtrlStartDescriptor desc;
        desc.pathPtr = "0:GNWSH.ELF";
        desc.pathLen = 11;
        desc.errorPtr = &err;
        k_scr_usr_start(KERNEL_PROC_ID, &desc);
    }

    if (err != GCE_NONE) {
        OOPS("Unable to start core modules");
    }
}
