//
//  math.c
//  GunwOS
//
//  Created by Artur Danielewski on 27.02.2020.
//

#include "include/math.h"

int pow(int base, int exp) {
    if (exp == 0) {
        return 1;
    }
    if (exp == 1) {
        return base;
    }
    
    int result = base;
    
    while (exp-- > 1) {
        result *= base;
    }

    return result;
}

// double pow(double base, int exp) {
//     if (exp == 0) {
//         return 1;
//     }
//     while (exp > 0 ? exp-- : exp++) {
//         exp > 0 ? base *= base : base /= base;
//     }

//     return base;
// }

int ceil(double val) {
    int iVal = (int)val;
    return (val < 0) ? iVal : (val > iVal) ? (iVal + 1) : iVal;
}
