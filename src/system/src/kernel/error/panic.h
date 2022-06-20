//
//  panic.h
//  GunwOS
//
//  Created by Artur Danielewski on 04.02.2021.
//

#include "../../../../lib/stdgunw/utils.h"

#define OOPS(REASON) {k_oops(REASON); __builtin_unreachable();}

void k_oops(const char *reason);