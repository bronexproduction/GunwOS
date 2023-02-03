//
//  panic.h
//  GunwOS
//
//  Created by Artur Danielewski on 04.02.2021.
//

#ifndef PANIC_H
#define PANIC_H

#include <stdgunw/utils.h>

#define OOPS(REASON) { k_oops(REASON); __builtin_unreachable(); }

void k_oops(const char *reason);

#endif // PANIC_H
