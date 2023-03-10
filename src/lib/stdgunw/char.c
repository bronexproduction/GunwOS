//
//  char.c
//  GunwOS
//
//  Created by Artur Danielewski on 15.02.2020.
//

#include "include/char.h"
#include "include/utils.h"

uint_8 charisnum(const char c, uint_8 base) {   /* Base supported up to 16 */
    if (!base || base > 16) {
        return 0;
    }

    if (IN_RANGE('a', c, ('a' + (base - 10) - 1)) ||
        IN_RANGE('A', c, ('A' + (base - 10) - 1))) {
        return 1;
    }

    return IN_RANGE('0', c, ('0' + base - 1)) && c <= '9';
}

uint_8 chartonum(const char c) {
    return IN_RANGE('0', c, '9') ? c - '0' :
           IN_RANGE('a', c, 'z') ? c - 'a' + 10 :
           IN_RANGE('A', c, 'Z') ? c - 'A' + 10 :
           0;           
}
