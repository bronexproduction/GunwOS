//
//  debugprint.c
//  GunwOS
//
//  Created by Artur Danielewski on 12.03.2023.
//

#include <syscall/func.h>
#include <hal/proc/proc.h>

size_t k_scr_usr_debugPrint(const char * const bufferPtr, const size_t bufferLen) {
    const procId_t procId = k_proc_getCurrentId();
    
    SCLF_GET_VALID_ABS(const ptr_t, bufferPtr, bufferLen, 0);
    
    #warning TO BE IMPLEMENTED - MISSING CHARACTER OUTPUT
    return bufferLen;
}
