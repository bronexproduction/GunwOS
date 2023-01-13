//
//  terminal.h
//  GunwOS
//
//  Created by Artur Danielewski on 19.01.2018.
//

#include "../../../../lib/stdgunw/types.h"

void k_trm_init();

int k_trm_putc(const char c);
int k_trm_puts(const char * const s);
int k_trm_putsl(const char * const s, unsigned int l);
int k_trm_putun_base(uint_32 i, uint_8 base);
int k_trm_putun(uint_32 i);
int k_trm_putun_h(uint_32 i);
int k_trm_putin(int_32 i);
