//
//  startup.c
//  GunwOS
//
//  Created by Artur Danielewski on 26.12.2024.
//

#include <gunwlog.h>
#include <gunwctrl.h>
#include <gunwdev.h>
#include <gunwfug.h>
#include <defs.h>
#include <string.h>

#define START(PATH) if (err == GCE_NONE) {          \
    log(PATH);                                      \
    procId_t procId;                                \
    err = start(PATH, &procId);                     \
    char procIdString[15];                          \
    int2str(procId, procIdString);                  \
    if (err == GCE_NONE) {                          \
        log("Loading successful. Process ID: ");    \
        log(procIdString);                          \
    }                                               \
}

#define INSTALL(PATH) if (installErr == GDRE_NONE && ctrlErr == GCE_NONE) { \
    log(PATH);                                                              \
    devInstall(PATH, &ctrlErr, &installErr);                                \
    if (ctrlErr == GCE_NONE) {                                              \
        log("Loading successful");                                          \
        if (installErr != GDRE_NONE) {                                      \
            log("Setup failed");                                            \
        }                                                                   \
    }                                                                       \
}

static void installCoreDrivers() {
    log("Installing core device drivers");
    
    enum gnwDriverError installErr = GDRE_NONE;
    enum gnwCtrlError ctrlErr = GCE_NONE;

    INSTALL("0:GNWVGA.GDV");    /* Display driver - process ID: 1 */

    if (installErr != GDRE_NONE || ctrlErr != GCE_NONE) {
        log("Unable to install core drivers");
        fug(FUG_OPERATION_FAILED);
    }
}

static void startCoreModules() {
    log("Starting core modules");

    enum gnwCtrlError err = GCE_NONE;

    START("0:GNWDISPM.ELF");    /* Display manager    - process ID: 2 */
    START("0:GNWKBDM.ELF");     /* Keyboard manager   - process ID: 3 */
    START("0:GNWTERM.ELF");     /* Terminal           - process ID: 4 */
    START("0:GNWSH.ELF");       /* Command line       - process ID: 5 */

    if (err != GCE_NONE) {
        log("Unable to start core modules");
        fug(FUG_OPERATION_FAILED);
    }
}

void dupa() {
    installCoreDrivers();
    startCoreModules();
}
