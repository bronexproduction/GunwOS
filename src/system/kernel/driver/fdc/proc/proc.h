//
//  proc.h
//  GunwOS
//
//  Created by Artur Danielewski on 26.03.2023.
//
//  Docs:
//  Intel 82077AA Datasheet 
//

#ifndef PROC_H
#define PROC_H

#include "../common/fdctypes.h"

enum fdc_opStatus proc_reset(const uint_16 base);

/*
    Recalibrating the drive
*/
enum fdc_opStatus proc_recalibrate(const uint_16 base, const uint_8 drive);

/*
    Error routine

    Disables controller and logs the error
*/
void proc_error(const uint_16 base, const enum fdc_opStatus s, const char * const reason);

/*
    Checks FDD on given drive index presence

    'present' set to 1 if given drive is present
*/
enum fdc_opStatus proc_fdcDetect(const uint_16 base, const uint_8 drive, uint_8 * const present);

/*
    Prepare drive for data command
*/
enum fdc_opStatus proc_prepare(const struct fdc_fddConfig config);

/*
    Turning the motor on/off on given drive
*/
enum fdc_opStatus proc_startMotor(const uint_16 base, const uint_8 drive);
enum fdc_opStatus proc_stopMotor(const uint_16 base, const uint_8 drive);

/*
    Sense interrupt status
*/
enum fdc_opStatus proc_senseInterruptStatus(const uint_16 base, uint_8 * const sr0, uint_8 * const pcn);

#endif // PROC_H
