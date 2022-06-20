//
//  cmd.h
//  GunwOS
//
//  Created by Artur Danielewski on 27.03.2020.
//
//  Docs:
//  Intel 82077AA Datasheet 
//

#include "../shared/types.h"

/*
    Control commands

    5.2 - Control commands
    (Intel 82077AA Datasheet)
*/

/*
    READ ID command (0x0A)
*/
enum k_fdc_opStatus fdc_cmd_readID(const uint_16 base, const uint_8 drive);

/*
    RECALIBRATE command (0x07)
*/
enum k_fdc_opStatus fdc_cmd_recalibrate(const uint_16 base, const uint_8 drive);

/*
    SEEK command

    Command code - 'mode' param
*/
enum k_fdc_opStatus fdc_cmd_seek(const uint_16 base, const enum fdc_seekMode);

/*
    SENSE INTERRUPT STATUS command (0x08)
*/
enum k_fdc_opStatus fdc_cmd_senseInterruptStatus(const uint_16 base);

/*
    SENSE DRIVE STATUS command (0x04)

    Result byte on data bus contains SR3:
    MSB                                       LSB
    | 0 | WP | 1 | TRK0 | 1 | HDSEL | DS1 | DS0 |
    
*/
enum k_fdc_opStatus fdc_cmd_senseDriveStatus(const uint_16 base, 
                                             const uint_8 hds,
                                             const uint_8 drive);

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
enum k_fdc_opStatus fdc_cmd_specify(const uint_16 base,
                                    const uint_8 srt,
                                    const uint_8 hut,
                                    const uint_8 hlt,
                                    const enum fdc_dmaMode);

/*
    CONFIGURE command (0x13)
*/
enum k_fdc_opStatus fdc_cmd_configure(const uint_16 base,
                                      const enum fdc_eis,
                                      const enum fdc_efifo,
                                      const enum fdc_poll,
                                      const uint_8 fifothr,
                                      const uint_8 pretrk);

/*
    VERSION command (0x10)
*/
enum k_fdc_opStatus fdc_cmd_version(const uint_16 base);

/*
    DUMPREG command (0x0E)
*/
enum k_fdc_opStatus fdc_cmd_dumpreg(const uint_16 base);

/*
    PERPENDICULAR MODE command (0x12)
*/
enum k_fdc_opStatus fdc_cmd_perpendicularMode(const uint_16 base);

/*
    Data transfer commands

    5.1 - Data transfer commands
    (Intel 82077AA Datasheet)
*/

/*
    READ command

    Command code - 'mode' parameter
*/
enum k_fdc_opStatus fdc_cmd_read(const uint_16 base,
                                 const enum fdc_readMode,
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
    WRITE command

    Command code - 'mode' parameter
*/
enum k_fdc_opStatus fdc_cmd_write(const uint_16 base,
                                  const enum fdc_writeMode,
                                  const uint_8 mt,
                                  const uint_8 mfm,
                                  const uint_8 hds,
                                  const uint_8 drive,
                                  const uint_8 c,
                                  const uint_8 h,
                                  const uint_8 r,
                                  const uint_8 n,
                                  const uint_8 eot,
                                  const uint_8 gpl,
                                  const uint_8 dtl);

/*
    VERIFY command (0x16)
*/
enum k_fdc_opStatus fdc_cmd_verify(const uint_16 base);

/*
    FORMAT TRACK command (0x0D)
*/
enum k_fdc_opStatus fdc_cmd_formatTrack(const uint_16 base);

/*
    SCAN command

    Command code - 'mode' parameter
*/
enum k_fdc_opStatus fdc_cmd_scan(const uint_16 base, const enum fdc_scanMode);

/*
    Command set enhancements

    5.3 - Command set enhancements
    (Intel 82077AA Datasheet)
*/

/*
    LOCK command (0x14)
*/
enum k_fdc_opStatus fdc_cmd_lock(const uint_16 base);
