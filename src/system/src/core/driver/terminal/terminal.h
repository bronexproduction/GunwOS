//
//  terminal.h
//  GunwOS
//
//  Created by Artur Danielewski on 19.01.2018.
//

#include <stdgunw/types.h>

void c_trm_init();

int c_trm_putc(const char c);
int c_trm_puts(const char * const s);
int c_trm_putsl(const char * const s, unsigned int l);
int c_trm_putun_base(uint_32 i, uint_8 base);
int c_trm_putun(uint_32 i);
int c_trm_putun_h(uint_32 i);
int c_trm_putin(int_32 i);
