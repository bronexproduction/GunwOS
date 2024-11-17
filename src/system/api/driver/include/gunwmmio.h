//
//  gunwmmio.h
//  GunwOS
//
//  Created by Artur Danielewski on 17.11.2024.
//

#ifndef GUNWOS_GUNWMMIO_H
#define GUNWOS_GUNWMMIO_H

#include <types.h>

/*
    Returns new virtual memory pointer mapped to requested physical memory address

    Params:
        * sizeBytes - size of the requested memory space
        * physMemStart - physical address of the begginning of the buffer

    Return: Pointer to the newly allocated memory or nullptr if anything went wrong
*/
extern ptr_t mmioPlz(const size_t sizeBytes, const addr_t physMemStart);

#endif // GUNWOS_GUNWMMIO_H
