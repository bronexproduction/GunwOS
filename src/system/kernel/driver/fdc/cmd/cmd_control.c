//
//  cmd_control.c
//  GunwOS
//
//  Created by Artur Danielewski on 26.03.2023.
//
//  Docs:
//  Intel 82077AA Datasheet 
//  5.2 - Control commands 
//

#include "cmd.h"
#include "../common/io.h"

enum fdc_opStatus cmd_version(const uint_16 base) {
    TRY(pushData(base, 0x10));
    
    return OPSTATUS_OK;
}
