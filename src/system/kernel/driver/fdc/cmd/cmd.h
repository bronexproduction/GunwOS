//
//  cmd.h
//  GunwOS
//
//  Created by Artur Danielewski on 26.03.2023.
//
//  Docs:
//  Intel 82077AA Datasheet 
//

#ifndef CMD_H
#define CMD_H

#include "../common/fdctypes.h"

#define TRY_MSG(CMD, MSG) { \
    enum fdc_opStatus s = CMD; \
    if (s != OPSTATUS_OK) { proc_error(base, s, MSG); return s; } \
}
#define TRY(CMD) { \
    enum fdc_opStatus s = CMD; \
    if (s != OPSTATUS_OK) { return s; } \
}

/*
    5.1 - Data transfer commands
*/

enum fdc_opStatus cmd_read(const uint_16 base, 
                           const enum fdc_readMode mode,
                           const uint_8 mt,
                           const uint_8 mfm,
                           const uint_8 sk,
                           const uint_8 drive,
                           const uint_8 c,
                           const uint_8 h, 
                           const uint_8 r,
                           const uint_8 n,
                           const uint_8 eot,
                           const uint_8 gpl,
                           const uint_8 dtl);

/*
    5.2 - Control commands 
*/

enum fdc_opStatus cmd_version(const uint_16 base);

#endif // CMD_H
