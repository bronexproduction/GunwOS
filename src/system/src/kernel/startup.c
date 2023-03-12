//
//  startup.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.03.2023.
//

#include <gunwctrl.h>

void k_startup() {
    start("cli");

    // ptr_t cliElf = (ptr_t)0x50000;
    // const struct elfHeader32 *headerPtr = (struct elfHeader32 *)cliElf;

    // (void)headerPtr;

    // int i = 0;
    // i++;
    // #warning TO BE IMPLEMENTED
}
