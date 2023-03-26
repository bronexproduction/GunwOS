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

/*
    Recalibrating the drive
*/
enum k_fdc_opStatus fdc_proc_recalibrate(const uint_16 base, const uint_8 drive);

/*
    Sense interrupt status
*/
enum k_fdc_opStatus fdc_proc_senseInterruptStatus(const uint_16 base, uint_8 * const sr0, uint_8 * const pcn);

/*
    Turning on the motor on given drive
*/
enum k_fdc_opStatus fdc_proc_startMotor(const uint_16 base, const uint_8 drive);

/*
    Turning off the motor on given drive
*/
enum k_fdc_opStatus fdc_proc_stopMotor(const uint_16 base, const uint_8 drive);
