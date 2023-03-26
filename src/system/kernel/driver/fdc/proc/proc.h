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

#endif // PROC_H
