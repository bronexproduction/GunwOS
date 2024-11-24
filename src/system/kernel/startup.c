//
//  startup.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.03.2023.
//

#include "startup.h"
#include <string.h>
#include <_gunwctrl.h>
#include <_gunwdev.h>
#include <hal/proc/proc.h>
#include <log/log.h>
#include <error/panic.h>
#include <gunwdevtypes.h>
#include <prog/prog.h>

#define INSTALL(PATH) if (installErr == GDIE_NONE && ctrlErr == GCE_NONE) {                                     \
    LOG(PATH);                                                                                                  \
    struct gnwDeviceInstallDescriptor desc = { { { (byte_t *)PATH, strlen(PATH) }, &ctrlErr }, &installErr };   \
    extern void k_scr_usr_devInstall(const procId_t, struct gnwDeviceInstallDescriptor * const);                \
    k_scr_usr_devInstall(KERNEL_PROC_ID, &desc);                                                                \
}

#define START(PATH) if (err == GCE_NONE) {                                  \
    LOG(PATH);                                                              \
    err = k_prog_spawnProgram((data_t){ (byte_t *)PATH, strlen(PATH) });    \
}

static void installCoreDrivers() {
    LOG("Installing core device drivers");
    
    enum gnwDeviceInstallError installErr = GDIE_NONE;
    enum gnwCtrlError ctrlErr = GCE_NONE;

    INSTALL("0:GNWVGA.GDV");    /* Display driver - process ID: 0 */

    if (installErr != GDIE_NONE || ctrlErr != GCE_NONE) {
        OOPS("Unable to install core drivers",);
    }
}

static void startCoreModules() {
    LOG("Starting core modules");

    enum gnwCtrlError err = GCE_NONE;

    START("0:GNWDISPM.ELF");    /* Display manager  - process ID: 1 */
    START("0:GNWKBDM.ELF");     /* Keyboard manager - process ID: 2 */
    START("0:GNWTERM.ELF");     /* Terminal         - process ID: 3 */
    START("0:GNWSH.ELF");       /* Command line     - process ID: 4 */

    if (err != GCE_NONE) {
        OOPS("Unable to start core modules",);
    }
}

void k_startup() {
    installCoreDrivers();
    startCoreModules();
}
