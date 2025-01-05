//
//  startup.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.03.2023.
//

#include "startup.h"
#include <string.h>
#include <_gunwctrl.h>
#include <log/log.h>
#include <error/panic.h>
#include <prog/prog.h>
#include <hal/proc/proc.h>

#define START(PATH) if (err == GCE_NONE) {                                                  \
    LOG(PATH);                                                                              \
    err = k_prog_spawnProgram(KERNEL_PROC_ID, (data_t){ (byte_t *)PATH, strlen(PATH) });    \
}

static void launchStartupManager() {
    LOG("Launching startup manager");

    enum gnwCtrlError err = GCE_NONE;

    START("0:GNWSTART.ELF");    /* System startup manager - process ID: 0 */

    if (err != GCE_NONE) {
        OOPS("Unable to start core modules",);
    }
}

void k_startup() {
    launchStartupManager();
}
