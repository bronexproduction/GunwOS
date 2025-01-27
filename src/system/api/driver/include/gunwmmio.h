//
//  gunwmmio.h
//  GunwOS
//
//  Created by Artur Danielewski on 17.11.2024.
//

#ifndef GUNWOS_GUNWMMIO_H
#define GUNWOS_GUNWMMIO_H

#include <types.h>
#include <gunwmemtypes.h>

/*
    Returns new virtual memory pointer mapped to requested physical memory address

    Params:
        * physMemStart - physical address of the begginning of the buffer
        * sizeBytes - size of the requested memory space
        * errorPtr - enum gnwMemoryError pointer

    Return: Pointer to the newly allocated memory or nullptr if anything went wrong
*/
extern ptr_t mmioPlz(const addr_t physMemStart, const size_t sizeBytes, enum gnwMemoryError * const errorPtr);

#endif // GUNWOS_GUNWMMIO_H
