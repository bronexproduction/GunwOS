//  gunwlog.h
//  GunwOS
//
//  Created by Artur Danielewski on 21.12.2023.
//

#ifndef GUNWOS_GUNWLOG_H
#define GUNWOS_GUNWLOG_H

#ifndef _GUNWAPI_KERNEL

/*
    Sends log to the kernel
*/
extern void log(const char * const msg);

#endif // _GUNWAPI_KERNEL

#endif // GUNWOS_GUNWLOG_H
