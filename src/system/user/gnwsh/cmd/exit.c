//
//  exit.c
//  GunwOS
//
//  Created by Artur Danielewski on 23.03.2023.
//

#include "../cmdutil.h"
#include <gunwctrl.h>

void cmd_exit(const char * const params) {
    CMD_NOPARAM

    bye(69);
}
