//  gunwoutput.h
//  GunwOS
//
//  Created by Artur Danielewski on 21.12.2023.
//

#ifndef GUNWOS_GUNWOUTPUT_H
#define GUNWOS_GUNWOUTPUT_H

#ifndef _GUNWAPI_KERNEL

/*
    Prints to default terminal
*/
extern void print(const char * const msg, ...);

/*
    Sends log to the kernel
*/
extern void log(const char * const msg, ...);

#endif // _GUNWAPI_KERNEL

#endif // GUNWOS_GUNWOUTPUT_H
