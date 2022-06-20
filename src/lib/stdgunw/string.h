//
//  string.h
//  GunwOS
//
//  Created by Artur Danielewski on 27.01.2018.
//

#include "types.h"

#define ASCII_DIGIT_OFFSET 48
#define ASCII_LOWERCASE_LETTER_OFFSET 97

size_t strlen(const char * const);
int_32 strcmp(const char * const, const char * const);
void strprefix(const char * const s, size_t l, char * const out);
void strreplace(char * s, char bef, char aft);
int_32 str2int(const char * const s, uint_8 * const err);
size_t uint2str(const uint_32 i, char * const s, const uint_8 base);
size_t int2str(const int_32 i, char * const s);
size_t uint2dec(const uint_32 i, char * const s);
size_t uint2hex(const uint_32 i, char * const s);
size_t uint2bin(const uint_32 i, char * const s);
