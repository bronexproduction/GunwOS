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

procId_t k_prog_spawnProgram(const procId_t procId,
                            const data_t pathData);
procId_t k_prog_spawnDriver(const procId_t procId,
                            const data_t pathData);

#endif // PROG_H
