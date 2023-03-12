//
//  start.c
//  GunwOS
//
//  Created by Artur Danielewski on 12.03.2023.
//

#include <gunwctrl.h>
#include <types.h>
#include <string.h>
#include <hal/mem/mem.h>
#include <hal/proc/proc.h>
#include <elf.h>

enum gnwCtrlError k_scr_usr_start(const char * const path, const size_t pathLen) {
    if (!path) {
        return GCE_INVALID_ARGUMENT;
    }

    procId_t procId = k_proc_getCurrentId();
    ptr_t absPathPtr = k_mem_absForProc(procId, (const ptr_t)path);

    if (!k_mem_bufInZoneForProc(procId, absPathPtr, pathLen)) {
        return GCE_INVALID_ARGUMENT;
    }

    char * l = "dupa";
    char * r = "dupka";

    int_32 strcmpRes = strcmp(l, r);

    ptr_t data;
    if (pathLen == 3 && strcmpl("cli", (const char *)absPathPtr, pathLen)) {
        data = (ptr_t)0x50000;
    } else {
        return GCE_NOT_FOUND;
    }

    const struct elfHeader32 *headerPtr = (struct elfHeader32 *)data;
    (void)headerPtr;
    (void)strcmpRes;

    // (void)headerPtr;

    // int i = 0;
    // i++;
    // #warning TO BE IMPLEMENTED

    #warning TO BE IMPLEMENTED
    return GCE_NONE;
}
