//
//  startup.c
//  GunwOS
//
//  Created by Artur Danielewski on 26.12.2024.
//

#include <gunwoutput.h>
#include <gunwctrl.h>
#include <gunwdev.h>
#include <gunwfug.h>
#include <string.h>

#define START(PATH) if (err == GCE_NONE) {                  \
    logl(PATH);                                              \
    procId_t procId;                                        \
    err = start(PATH, &procId);                             \
    if (err == GCE_NONE) {                                  \
        logl("Loading successful. Process ID: {i}", procId); \
    }                                                       \
}

#define INSTALL(PATH) if (installErr == GDRE_NONE && ctrlErr == GCE_NONE) { \
    logl(PATH);                                                              \
    devInstall(PATH, &ctrlErr, &installErr);                                \
    if (ctrlErr == GCE_NONE) {                                              \
        logl("Loading successful");                                          \
        if (installErr != GDRE_NONE) {                                      \
            logl("Setup failed");                                            \
        }                                                                   \
    }                                                                       \
}

static void installCoreDrivers() {
    logl("Installing core device drivers");
    
    enum gnwDriverError installErr = GDRE_NONE;
    enum gnwCtrlError ctrlErr = GCE_NONE;

    INSTALL("0:GNWVGA.GDV");    /* Display driver - process ID: 1 */
    INSTALL("0:GNWINPUT.GDV");  /* Keyboard and mouse driver - process ID: 2 */

    if (installErr != GDRE_NONE || ctrlErr != GCE_NONE) {
        logl("Unable to install core drivers");
        fug(FUG_OPERATION_FAILED);
    }
}

static void startCoreModules() {
    logl("Starting core modules");

    enum gnwCtrlError err = GCE_NONE;

    START("0:GNWDISPM.ELF");    /* Display manager    - process ID: 3 */
    START("0:GNWINPTM.ELF");    /* Input manager      - process ID: 4 */
    START("0:GNWTERM.ELF");     /* Terminal           - process ID: 5 */
    START("0:GNWSH.ELF");       /* Command line       - process ID: 6 */

    if (err != GCE_NONE) {
        logl("Unable to start core modules");
        fug(FUG_OPERATION_FAILED);
    }
}

void dupa() {
    installCoreDrivers();
    startCoreModules();
}
