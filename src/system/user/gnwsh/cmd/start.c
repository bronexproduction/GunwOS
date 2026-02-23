//
//  start.c
//  GunwOS
//
//  Created by Artur Danielewski on 23.03.2023.
//

#include "../cmdutil.h"
#include <gunwctrl.h>
#include <gunwoutput.h>
#include <defs.h>

void cmd_start(const char * const params) {
    CMD_PARAM_STRING(0, name, "Program name")

    enum gnwCtrlError err = start(name, nullptr);
    switch (err) {
    case GCE_NONE:
        break;
    case GCE_NOT_FOUND:
        print("Program ");
        print(name);
        print(" not found");
        break;
    default:
        print("Error starting ");
        print(name);
        break;
    }
}
