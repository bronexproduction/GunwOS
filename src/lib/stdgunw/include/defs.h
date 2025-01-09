//
//  defs.h
//  GunwOS
//
//  Created by Artur Danielewski on 15.01.2023.
//

#ifndef STDGUNW_DEFS_H
#define STDGUNW_DEFS_H

#define NULL 0
#define nullptr 0

#define KiB(C) ((C) * 1024)
#define MiB(C) (KiB(C) * 1024)
#define GiB(C) (MiB(C) * 1024)

#define PATH_LENGTH_LIMIT 255

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))

#endif // STDGUNW_DEFS_H
