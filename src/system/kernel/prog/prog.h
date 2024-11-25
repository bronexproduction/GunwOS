//
//  prog.h
//  GunwOS
//
//  Created by Artur Danielewski on 24.11.2024.
//

#ifndef PROG_H
#define PROG_H

#include <types.h>
#include <gunwdevtypes.h>

enum gnwCtrlError k_prog_spawnProgram(const data_t pathData);
enum gnwCtrlError k_prog_spawnDriver(const data_t pathData,
                                     enum gnwDeviceInstallError * const installError);

#endif // PROG_H
