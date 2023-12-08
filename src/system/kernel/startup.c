//
//  startup.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.03.2023.
//

#include <string.h>
#include <src/_gunwctrl.h>
#include <hal/proc/proc.h>
#include <error/panic.h>

#define START(PATH) if (err == GCE_NONE) {                                                      \
    struct gnwCtrlStartDescriptor desc;                                                         \
    desc.pathPtr = PATH;                                                                        \
    desc.pathLen = strlen(desc.pathPtr);                                                        \
    desc.errorPtr = &err;                                                                       \
    extern void k_scr_usr_start(const procId_t, const struct gnwCtrlStartDescriptor * const);   \
    k_scr_usr_start(KERNEL_PROC_ID, &desc);                                                     \
}

void k_startup() {
    
    enum gnwCtrlError err = GCE_NONE;

    START("0:GNWDISPM.ELF");    /* Display manager */
    START("0:GNWKBDM.ELF");     /* Keyboard manager */
    START("0:GNWTERM.ELF");     /* Terminal */
    START("0:GNWSH.ELF");       /* Command line */

    if (err != GCE_NONE) {
        OOPS("Unable to start core modules");
    }
}
