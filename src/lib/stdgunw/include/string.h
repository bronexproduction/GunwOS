//
//  string.h
//  GunwOS
//
//  Created by Artur Danielewski on 27.01.2018.
//

#include "types.h"

#define ASCII_DIGIT_OFFSET 48
#define ASCII_LOWERCASE_LETTER_OFFSET 97

#define STR_NOT_FOUND -1

/*
    Returns the length of 0-terminated character array

    The '\0' character is not included in calculation result,
    therefore buffer length is always 1 byte greater than the result
*/
size_t strlen(const char * const);
int_32 strcmp(const char * const, const char * const);
int_32 strcmpl(const char * const, const char * const, const size_t);
void strprefix(const char * const s, size_t l, char * const out);
void strreplace(char * const s, const char bef, const char aft);
void strreplacel(char * const s, const size_t sLen, const char bef, const char aft);

int_32 strfindl(const char * const where, const int_32 whereLen, const char * const what, const int_32 whatLen);

int_32 str2int(const char * const s, bool * const err);
int_32 str2intl(const char * const s, const size_t sLen, bool * const err);
size_t uint2str(const uint_32 i, char * const s, const uint_8 base);
size_t int2str(const int_32 i, char * const s);
size_t uint2dec(const uint_32 i, char * const s);
size_t uint2hex(const uint_32 i, char * const s);
size_t uint2bin(const uint_32 i, char * const s);
