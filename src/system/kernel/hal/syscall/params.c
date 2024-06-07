//
//  params.c
//  GunwOS
//
//  Created by Artur Danielewski on 23.06.2024.
//

#include "params.h"

void * userStackSafeValuePointer(ptr_t refEsp, addr_t offset) {
    #warning TO BE GUARDED
    return _USER_STACK_PTR + offset;
}
