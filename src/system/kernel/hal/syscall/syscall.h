//
//  syscall.h
//  GunwOS
//
//  Created by Artur Danielewski on 12.02.2024.
//

#ifndef SYSCALL_H
#define SYSCALL_H

#include <types.h>

void k_scl_syscall_DRIVER(const ptr_t refEsp);
void k_scl_syscall_USER(const ptr_t refEsp);

#endif // SYSCALL_H
