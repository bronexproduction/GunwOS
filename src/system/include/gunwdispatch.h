//
//  gunwdispatch.h
//  GunwOS
//
//  Created by Artur Danielewski on 02.02.2021.
//

#ifndef GUNWOS_GUNWDISPATCH_H
#define GUNWOS_GUNWDISPATCH_H

#include <stdgunw/types.h>

/*
    Function dispatch descriptor

    Used to queue function execution from run loop
*/
struct gnwDispatchDesc {
    
    void (*function_u8)(uint_8 param);
    uint_32 param1;
};

#endif // GUNWOS_GUNWDISPATCH_H
