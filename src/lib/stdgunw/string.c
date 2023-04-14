//
//  string.c
//  GunwOS
//
//  Created by Artur Danielewski on 27.01.2018.
//

#include "include/string.h"

#include "include/char.h"
#include "include/math.h"
#include "include/mem.h"

size_t strlen(const char * const c) {
    size_t s = 0;
    while (*(c+s)) ++s;
    return s;
}

int_32 strcmp(const char * const l, const char * const r) {
    size_t ll = strlen(l);
    size_t rl = strlen(r);
    return ll < rl ? 1 : ll > rl ? -1 : strcmpl(l, r, ll);
}

int_32 strcmpl(const char * const l, const char * const r, const size_t len) {
    if (!len) { return 0; }
    size_t s = 0;
    while (*(l + s) == *(r + s) /* characters equal */ &&
           *(l + s) && *(r + s) /* characters not 0 */ &&
           ((s + 1) < len) /* not last character */) ++s;

    if (*(l + s) == *(r + s)) return 0;

    return *(l + s) > *(r + s) ? -1 : 1;
}

void strprefix(const char * const s, size_t l, char * const out) {
    if (!l || !out) {
        return;
    }

    memnull(out, (l + 1) * sizeof(char));

    for (size_t i = 0; i < l; ++i) {
        if (s[i]) {
            out[i] = s[i];
        } else break;
    }
}

void strreplace(char * const s, const char bef, const char aft) {
    strreplacel(s, strlen(s), bef, aft);
}

void strreplacel(char * const s, const size_t sLen, const char bef, const char aft) {
    for (size_t i = 0; i < sLen; ++i) {
        if (*(s + i) == bef) {
            *(s + i) = aft;
        }
    }
}

int_32 strfindl(const char * const where, const int_32 whereLen, const char * const what, const int_32 whatLen) {
    if ((whereLen <= 0) || (whatLen <= 0) || (whatLen > whereLen)) {
        return STR_NOT_FOUND;
    }
    
    int_32 whatLookup = 0;
    int_32 currentIndex = 0;
    const int_32 indexOfLastHope = whereLen - whatLen;

    for (; (currentIndex < whereLen) && (whatLookup != whatLen); ++currentIndex) {
        if (where[currentIndex] == what[whatLookup]) {
            ++whatLookup;
        } else {
            if (whatLookup) --currentIndex;
            if (currentIndex >= indexOfLastHope) break;
            whatLookup = 0;
        }
    }

    return (whatLookup == whatLen) ? (currentIndex - whatLen) : STR_NOT_FOUND;
}

int_32 str2int(const char * const c, bool * const err) {
    return str2intl(c, strlen(c), err);
}

int_32 str2intl(const char * const s, const size_t sLen, bool * const err) {
    if (!sLen) {
        if (err) {
            *err = true;
        }
        return 0;
    }

    int_8 negative = 0;     // is negative
    size_t offset = 0;      // offset from start where the actual number begins
    uint_8 base;            // numeral system base

    char prefixBuffer[3];
    memnull(prefixBuffer, 3);
    if (sLen >= 2) {
        strprefix(s, 2, prefixBuffer);
    }

    if (!strcmp(prefixBuffer, "0x")) {            // Hex
        base = 16;
        offset = 2;
    } else if (!strcmp(prefixBuffer, "0b")) {     // Binary
        base = 2;
        offset = 2;
    } else {                                        // Decimal
        if (*s == '-') {                            // Negative decimal
           offset = 1;
           negative = 1;
        }

        base = 10;
    }

    size_t len = sLen - offset;     // number length
    if (!len) {                     // if there is no number
        if (err) {                  // and if error pointer is set
            *err = true;            // set error flag
        }

        return 0;
    }

    int_32 result = 0;

    for (size_t i = 0; i < len; i++) {
        char curr = (s+offset)[i];
        
        if (!charisnum(curr, base)) {   // Check if character is correct numeral
            if (err) {                  // if not and if error pointer is set
                *err = true;               // set error flag
            }
            
            return 0;
        }
        
        result += chartonum(curr) * pow(base, len - 1 - i);
    }

    if (err) {
        *err = false;
    }

    return negative ? -result : result;
}

size_t uint2str(const uint_32 i, char * const s, const uint_8 base) {
    if (!s) return 0;

    int length = 0;
    int b = i;
    int exp = -1;

    do {
        b /= base;
        exp++;
    } while (b > 0);

    for (; exp >= 0; --exp) {
        int digit = i / pow(base, exp) % base;
        char character = (digit < 10) ? (digit + ASCII_DIGIT_OFFSET) : ((digit - 10) + ASCII_LOWERCASE_LETTER_OFFSET);

        *(s + length++) = character;
    }

    return length;
}

size_t int2str(const int_32 i, char * const s) {
    if (!s) return 0;

    if (i < 0) {
        *s = '-';
        return uint2str(i * -1, s + 1, 10) + 1;
    }
    else {
        return uint2str(i, s, 10);
    }
}

size_t uint2dec(const uint_32 i, char * const s) {
    return uint2str(i, s, 10);
}

size_t uint2hex(const uint_32 i, char * const s) {
    return uint2str(i, s, 16);
}

size_t uint2bin(const uint_32 i, char * const s) {
    return uint2str(i, s, 2);
}
