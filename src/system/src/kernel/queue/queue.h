//
//  queue.h
//  GunwOS
//
//  Created by Artur Danielewski on 30.01.2023.
//

#ifndef QUEUE_H
#define QUEUE_H

#include <types.h>

typedef __attribute__((cdecl)) void (*fPtr_void)();
typedef __attribute__((cdecl)) void (*fPtr_arch_arch)(addr_t, addr_t);

/*
    Functions dedicated for additional time-consuming operations
    interrupt handlers may require to perform to process data

    To keep interrupt service routines simple
    ISR can queue its time-consuming task
    to be executed after ISR returns
*/
void k_que_dispatch(const fPtr_void func);
void k_que_dispatch_arch_arch(const fPtr_arch_arch func, const addr_t p0, const addr_t p1);

#endif // QUEUE_H
