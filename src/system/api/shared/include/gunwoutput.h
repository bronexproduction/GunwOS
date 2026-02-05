//  gunwoutput.h
//  GunwOS
//
//  Created by Artur Danielewski on 21.12.2023.
//

#ifndef GUNWOS_GUNWOUTPUT_H
#define GUNWOS_GUNWOUTPUT_H

#include <types.h>

/*
    Formatted output handling

    Arguments data types are to be written in curly brackets
    Optional format information is to be put in square brackets and should follow the curly brackets
    (escape character required in case curly bracket or square bracket is to be put as a character instead)

    How to declare a parameter in msg:

        Format of expected parameter is: {T} or {T:F} where:

        * T - data type (see table below)
        * F - optional formatting instructions (see table below)
    
    Supported data types:

        * u - unsigned integer (default platform size)
        * h - unsigned integer (default platform size) as hexadecimal
        * i - signed integer (default platform size)

    Optional formatting instructions:

        * NONE

    Examples of formatted msg:

        * "Your pizza is {u8} cm in diameter, you will stay hungry"
        * "My pizza is {u16] cm in diameter, it's much bigger than yours"
        * "His pizza is {u32} cm in diameter, he must have stolen it"
*/

#ifndef _GUNWAPI_KERNEL

/*
    Prints to default terminal
*/
extern size_t printc(const char c);
extern size_t print(const char * msg);
extern size_t printn(const char * msg);
extern size_t printl(const char * msg, const size_t l);
extern size_t println(const char * msg, const size_t l);
extern size_t printf(const char * msg, ...);
extern size_t printfn(const char * msg, ...);
extern size_t printfl(const char * msg, const size_t l, ...);
extern size_t printfln(const char * msg, const size_t l, ...);

#endif // _GUNWAPI_KERNEL

/*
    Sends log to the kernel
*/
extern size_t logc(const char c);
extern size_t log(const char * msg);
extern size_t logn(const char * msg);
extern size_t logl(const char * msg, const size_t l);
extern size_t logln(const char * msg, const size_t l);
extern size_t logf(const char * msg, ...);
extern size_t logfn(const char * msg, ...);
extern size_t logfl(const char * msg, const size_t l, ...);
extern size_t logfln(const char * msg, const size_t l, ...);

#endif // GUNWOS_GUNWOUTPUT_H
