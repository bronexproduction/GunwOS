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
void k_prog_spawnDriver(const procId_t procId,
                        const data_t * const vPathDataPtr,
                        enum gnwDeviceInstallError * const vInstallErrorPtr,
                        enum gnwCtrlError * const vCtrlErrorPtr);

#endif // PROG_H
