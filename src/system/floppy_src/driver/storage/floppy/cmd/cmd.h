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
    SEEK command

    Command code - 'mode' param
*/
enum k_fdc_opStatus fdc_cmd_seek(const uint_16 base, const enum fdc_seekMode);

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
    DUMPREG command (0x0E)
*/
enum k_fdc_opStatus fdc_cmd_dumpreg(const uint_16 base);

/*
    PERPENDICULAR MODE command (0x12)
*/
enum k_fdc_opStatus fdc_cmd_perpendicularMode(const uint_16 base);

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
