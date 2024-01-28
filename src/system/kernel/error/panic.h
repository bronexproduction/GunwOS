//
//  panic.h
//  GunwOS
//
//  Created by Artur Danielewski on 04.02.2021.
//

#ifndef PANIC_H
#define PANIC_H

#include <utils.h>
#include <hal/cpu/cpu.h>

#define OOPS_WITH_CODE_NBR(REASON) { __asm__ volatile ("pushl %[rsn]" : : [rsn] "r" (REASON)); __asm__ volatile ("call k_oops"); }
#define OOPS_WITH_CODE(REASON, RETURN) { OOPS_WITH_CODE_NBR(REASON); return RETURN; }

#define OOPS_NBR(REASON) { __asm__ volatile ("pushl $-1"); OOPS_WITH_CODE_NBR(REASON); }
#define OOPS(REASON, RETURN) { OOPS_NBR(REASON); return RETURN; }


void __attribute__((cdecl)) k_oops(const char *reason, const uint_32 errCode);

#endif // PANIC_H
