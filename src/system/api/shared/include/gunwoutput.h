//  gunwoutput.h
//  GunwOS
//
//  Created by Artur Danielewski on 21.12.2023.
//

#ifndef GUNWOS_GUNWOUTPUT_H
#define GUNWOS_GUNWOUTPUT_H

#ifndef _GUNWAPI_KERNEL

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
        * i - signed integer (default platform size)

    Optional formatting instructions:

        * NONE

    Examples of formatted msg:

        * "Your pizza is {u8} cm in diameter, you will stay hungry"
        * "My pizza is {u16] cm in diameter, it's much bigger than yours"
        * "His pizza is {u32} cm in diameter, he must have stolen it"
*/

/*
    Prints to default terminal
*/
extern void print(const char * msg, ...);
extern void printl(const char * msg, ...);

/*
    Sends log to the kernel
*/
extern void log(const char * msg, ...);
extern void logl(const char * msg, ...);

#endif // _GUNWAPI_KERNEL

#endif // GUNWOS_GUNWOUTPUT_H
