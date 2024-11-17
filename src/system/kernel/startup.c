//
//  startup.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.03.2023.
//

#include "startup.h"
#include <string.h>
#include <_gunwctrl.h>
#include <hal/proc/proc.h>
#include <log/log.h>
#include <error/panic.h>
#include <gunwdevtypes.h>

#define LOAD(TYPE, PATH, DESC, NONE_ERROR) if (err == NONE_ERROR) {                                                   \
    LOG(PATH);                                                                                          \
    struct gnw ## DESC ## Descriptor desc;                                                              \
    desc.pathPtr = PATH;                                                                                \
    desc.pathLen = strlen(desc.pathPtr);                                                                \
    desc.errorPtr = &err;                                                                               \
    extern void k_scr_usr_ ## TYPE (const procId_t, const struct gnw ## DESC ## Descriptor * const);    \
    k_scr_usr_ ## TYPE (KERNEL_PROC_ID, &desc);                                                         \
}

#define INSTALL(PATH) LOAD(devInstall, PATH, DeviceInstall, GDIE_NONE)
#define START(PATH) LOAD(start, PATH, CtrlStart, GCE_NONE)

static void installCoreDrivers() {
    LOG("Installing core device drivers");
    
    enum gnwDeviceInstallError err = GDIE_NONE;

    INSTALL("0:GNWVGA.GDV");    /* Display driver - process ID: 0 */

    if (err != GDIE_NONE) {
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
