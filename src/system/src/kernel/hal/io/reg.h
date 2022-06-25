//
//  reg.h
//  GunwOS
//
//  Created by Artur Danielewski on 06.01.2021.
//  Mostly copied & pasted from OS-dev wiki
//

#include "../../../../../lib/stdgunw/types.h"

static inline uint_64 k_reg_rdmsr(uint_32 msr_id) {
    uint_64 msr_value;
    __asm__ volatile ( "rdmsr"
                     : "=A" (msr_value) 
                     : "c" (msr_id) );

    return msr_value;
}
