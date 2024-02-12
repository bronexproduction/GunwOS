//
//  syscall.h
//  GunwOS
//
//  Created by Artur Danielewski on 12.02.2024.
//

#ifndef SYSCALL_H
#define SYSCALL_H

__attribute__((naked)) void k_scl_syscall_DRIVER();
__attribute__((naked)) void k_scl_syscall_USER();

#endif // SYSCALL_H
