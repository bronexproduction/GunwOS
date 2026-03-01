//
//  install.c
//  GunwOS
//
//  Created by Artur Danielewski on 01.03.2026.
//

#include "../cmdutil.h"
#include <gunwdev.h>
#include <gunwoutput.h>
#include <defs.h>

void cmd_install(const char * const params) {
    CMD_PARAM_STRING(0, path, "Driver path")

    enum gnwDriverError installErr = GDRE_NONE;
    enum gnwCtrlError ctrlErr = GCE_NONE;

    devInstall(path, &ctrlErr, &installErr);
    if (ctrlErr != GCE_NONE) {
        printfn("Driver loading failed with error: {i}", ctrlErr);
        return;
    }
    if (installErr != GDRE_NONE) {
        printfn("Driver installation failed with error: {i}", installErr);
        return;
    }

    printn("Installation successful");
}
