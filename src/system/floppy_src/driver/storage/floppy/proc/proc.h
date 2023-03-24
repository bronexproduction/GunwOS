//
//  proc.h
//  GunwOS
//
//  Created by Artur Danielewski on 29.03.2020.
//
//  Docs:
//  Intel 82077AA Datasheet 
//

#include "../shared/types.h"

#define TRY(CMD, MSG) { \
    enum k_fdc_opStatus s = CMD; \
    if (s != OPSTATUS_OK) { fdc_proc_error(base, s, MSG); return s; } \
}
#define TRY(CMD) { \
    enum k_fdc_opStatus s = CMD; \
    if (s != OPSTATUS_OK) { return s; } \
}

/*
    Error routine

    Disables controller and calls LOG_ERROR
*/
void fdc_proc_error(const uint_16 base, const enum k_fdc_opStatus s, const char * const reason);

/*
    Checks FDD on given drive index presence

    'present' set to 1 if given drive is present
*/
enum k_fdc_opStatus fdc_proc_fdcDetect(const uint_16 base, const uint_8 drive, uint_8 * const present);

/*
    Recalibrating the drive
*/
enum k_fdc_opStatus fdc_proc_recalibrate(const uint_16 base, const uint_8 drive);

/*
    Sense interrupt status
*/
enum k_fdc_opStatus fdc_proc_senseInterruptStatus(const uint_16 base, uint_8 * const sr0, uint_8 * const pcn);

/*
    Prepare drive for data command
*/
enum k_fdc_opStatus fdc_proc_prepare(const struct fdc_fddConfig config);

/*
    Turning on the motor on given drive
*/
enum k_fdc_opStatus fdc_proc_startMotor(const uint_16 base, const uint_8 drive);

/*
    Turning off the motor on given drive
*/
enum k_fdc_opStatus fdc_proc_stopMotor(const uint_16 base, const uint_8 drive);
