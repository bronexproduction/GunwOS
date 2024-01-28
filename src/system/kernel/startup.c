//
//  startup.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.03.2023.
//

#include <string.h>
#include <src/_gunwctrl.h>
#include <hal/proc/proc.h>
#include <log/log.h>
#include <error/panic.h>

#define START(PATH) if (err == GCE_NONE) {                                                      \
    LOG(PATH);                                                                                  \
    struct gnwCtrlStartDescriptor desc;                                                         \
    desc.pathPtr = PATH;                                                                        \
    desc.pathLen = strlen(desc.pathPtr);                                                        \
    desc.errorPtr = &err;                                                                       \
    extern void k_scr_usr_start(const procId_t, const struct gnwCtrlStartDescriptor * const);   \
    k_scr_usr_start(KERNEL_PROC_ID, &desc);                                                     \
}

void k_startup() {

    LOG("Starting core modules");

    enum gnwCtrlError err = GCE_NONE;

    START("0:GNWDISPM.ELF");    /* Display manager  - process ID: 0 */
    START("0:GNWKBDM.ELF");     /* Keyboard manager - process ID: 1 */
    START("0:GNWTERM.ELF");     /* Terminal         - process ID: 2 */
    START("0:GNWSH.ELF");       /* Command line     - process ID: 3 */

    if (err != GCE_NONE) {
        OOPS("Unable to start core modules",);
    }
}
