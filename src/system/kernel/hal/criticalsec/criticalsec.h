//
//  criticalsec.h
//  GunwOS
//
//  Created by Artur Danielewski on 01.02.2023.
//

#ifndef CRITICALSEC_H
#define CRITICALSEC_H

#define CRITICAL_SECTION_BEGIN {        \
    CPU_INTERRUPTS_DISABLE;             \
}
#define CRITICAL_SECTION_END {          \
    extern const size_t isrStackHeight; \
    if (!isrStackHeight) {              \
        CPU_INTERRUPTS_ENABLE;          \
    }                                   \
}
#define CRITICAL_SECTION(CODE) { CRITICAL_SECTION_BEGIN; { CODE; } CRITICAL_SECTION_END; }

#endif // CRITICALSEC_H
