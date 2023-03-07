//
//  fug.c
//  GunwOS
//
//  Created by Artur Danielewski on 18.02.2023.
//

#include "fug.h"
#include <stdgunw/string.h>
#include <error/panic.h>

void k_err_fug(enum gnwFugCode code) {
    char log[20] = "FUG code ";

    uint2hex(code, log + 9);
    OOPS(log);
}
