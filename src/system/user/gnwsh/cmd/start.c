//
//  start.c
//  GunwOS
//
//  Created by Artur Danielewski on 23.03.2023.
//

#include "../cmdutil.h"
#include <gunwctrl.h>
#include <gunwoutput.h>
#include <gunwfug.h>
#include <defs.h>
#include <proc.h>

void cmd_start(const char * const params) {
    CMD_PARAM_STRING(0, name, "Program name")

    procId_t procId;
    enum gnwCtrlError err = start(name, &procId);
    switch (err) {
    case GCE_NONE:
        {
            const enum gnwCtrlError waitError = waitForProc(procId);
            if (waitError != GCE_NONE) {
                logfn("Process execution failure with error code {i}, please cry.", waitError);
                fug(FUG_INCONSISTENT);
                return;
            }
        }
        break;
    case GCE_NOT_FOUND:
        print("Program ");
        print(name);
        printn(" not found");
        break;
    default:
        print("Error starting ");
        printn(name);
        break;
    }
}
