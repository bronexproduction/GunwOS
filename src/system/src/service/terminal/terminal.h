//
//  terminal.h
//  GunwOS
//
//  Created by Artur Danielewski on 19.01.2018.
//

#include "../../../../lib/stdgunw/types.h"

void s_trm_init();

int s_trm_putc(const char c);
int s_trm_puts(const char * const s);
int s_trm_putsl(const char * const s, unsigned int l);
int s_trm_putun_base(uint_32 i, uint_8 base);
int s_trm_putun(uint_32 i);
int s_trm_putun_h(uint_32 i);
int s_trm_putin(int_32 i);
