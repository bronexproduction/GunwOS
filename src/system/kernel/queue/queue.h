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
typedef __attribute__((cdecl)) void (*fPtr_arch)(addr_t);
typedef __attribute__((cdecl)) void (*fPtr_arch2)(addr_t, addr_t);
typedef __attribute__((cdecl)) void (*fPtr_arch4)(addr_t, addr_t, addr_t, addr_t);

void k_que_start();

/*
    Functions dedicated for additional time-consuming operations
    interrupt handlers may require to perform to process data

    To keep interrupt service routines simple
    ISR can queue its time-consuming task
    to be executed after ISR returns
*/
void k_que_dispatch(const fPtr_void func);
void k_que_dispatch_arch(const fPtr_arch func, const addr_t p0);
void k_que_dispatch_arch2(const fPtr_arch2 func, const addr_t p0, const addr_t p1);
void k_que_dispatch_arch4(const fPtr_arch4 func, const addr_t p0, const addr_t p1, const addr_t p2, const addr_t p3);

#endif // QUEUE_H
