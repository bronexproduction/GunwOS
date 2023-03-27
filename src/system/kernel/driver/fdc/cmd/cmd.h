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
    Data transfer commands

    5.1 - Data transfer commands
    (Intel 82077AA Datasheet)
*/

/*
    READ command

    Command code - 'mode' parameter
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

/*
    SPECIFY command (0x03)

    The SPECIFY command sets the initial values
    for each of the three internal timers:
    * HUT (Head Unload Time)
    * SRT (Step Rate Time)
    * HLT (Head Load Time)
    
    For the exact values refer to
    5.2.6 SPECIFY
    Intel 82077AA Datasheet
*/  
enum fdc_opStatus cmd_specify(const uint_16 base,
                              const uint_8 srt,
                              const uint_8 hut,
                              const uint_8 hlt,
                              const enum fdc_dmaMode);

/*
    RECALIBRATE command (0x07)
*/
enum fdc_opStatus cmd_recalibrate(const uint_16 base, const uint_8 drive);

/*
    SENSE INTERRUPT STATUS command (0x08)
*/
enum fdc_opStatus cmd_senseInterruptStatus(const uint_16 base);

/*
    VERSION command (0x10)
*/
enum fdc_opStatus cmd_version(const uint_16 base);

/*
    CONFIGURE command (0x13)
*/
enum fdc_opStatus cmd_configure(const uint_16 base,
                                const enum fdc_eis,
                                const enum fdc_efifo,
                                const enum fdc_poll,
                                const uint_8 fifothr,
                                const uint_8 pretrk);

#endif // CMD_H
