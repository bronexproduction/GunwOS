//
//  criticalsec.h
//  GunwOS
//
//  Created by Artur Danielewski on 01.02.2023.
//

#ifndef CRITICALSEC_H
#define CRITICALSEC_H

#define CRITICAL_SECTION_BEGIN { __asm__ volatile ("cli"); }
#define CRITICAL_SECTION_END { __asm__ volatile ("sti"); }
#define CRITICAL_SECTION(CODE) { CRITICAL_SECTION_BEGIN; { CODE; } CRITICAL_SECTION_END; }

#endif // CRITICALSEC_H
