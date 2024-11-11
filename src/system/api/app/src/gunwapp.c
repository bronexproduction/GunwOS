//
//  gunwapp.c
//  GunwOS
//
//  Created by Artur Danielewski on 24.03.2023.
//

#ifndef _GUNWAPI_KERNEL

#include "../include/gunwctrl.h"

extern void dupa();

__attribute__((cdecl)) void _start(const addr_t heap) {

    extern addr_t _heap;
    _heap = heap;
    
    dupa();
    bye(0);
    __builtin_unreachable();
}

#endif // _GUNWAPI_KERNEL
