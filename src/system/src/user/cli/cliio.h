//
//  cliio.h
//  GunwOS
//
//  Created by Artur Danielewski on 07.03.2023.
//

#ifndef CLIIO_H
#define CLIIO_H

#include <types.h>

extern bool user_cli_charOutAttached;
extern size_t user_cli_charOutIdentifier;

int user_cli_putc(const char c);
int user_cli_puts(const char * const s);
int user_cli_putsl(const char * const s, unsigned int l);
int user_cli_putun_base(uint_32 i, uint_8 base);
int user_cli_putun(uint_32 i);
int user_cli_putun_h(uint_32 i);
int user_cli_putin(int_32 i);

#endif // CLIIO_H
