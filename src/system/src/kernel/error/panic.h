//
//  panic.h
//  GunwOS
//
//  Created by Artur Danielewski on 04.02.2021.
//

#ifndef PANIC_H
#define PANIC_H

#include <utils.h>

#define OOPS_WITH_CODE(REASON) { __asm__ volatile ("pushl %[rsn]" : : [rsn] "r" (REASON)); __asm__ volatile ("call k_oops"); __builtin_unreachable(); }
#define OOPS(REASON) { __asm__ volatile ("pushl $-1"); OOPS_WITH_CODE(REASON); }

void __attribute__((cdecl)) k_oops(const char *reason, const uint_32 errCode);

#endif // PANIC_H
