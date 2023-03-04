//
//  func.h
//  GunwOS
//
//  Created by Artur Danielewski on 03.03.2023.
//

#ifndef FUNC_H
#define FUNC_H

#define SCR(NAME, CODE) __attribute__((naked)) void k_scr_ ## NAME () { CODE; __asm__ volatile ("ret"); }
#define SCR_STACK(NAME) void __attribute__((cdecl)) k_scr_ ## NAME () 

#endif // FUNC_H
