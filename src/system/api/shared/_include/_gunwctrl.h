//  _gunwctrl.h
//  GunwOS
//
//  Created by Artur Danielewski on 12.05.2023.
//

#ifndef GUNWOS__GUNWCTRL_H
#define GUNWOS__GUNWCTRL_H

#include "../include/gunwctrl.h"
#include <types.h>
#include "../include/gunwctrltypes.h"

enum gnwCtrlExecType {
    GET_PROGRAM,
    GET_DRIVER
};

struct gnwCtrlStartDescriptor {
    const data_t pathData;
    const enum gnwCtrlExecType type;
    procId_t * procIdOrErrorPtr; /* procId_t (if >=0) or enum gnwCtrlError (if <0) */
};

#endif // GUNWOS__GUNWCTRL_H
