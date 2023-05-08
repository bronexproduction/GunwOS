//
//  debugprint.c
//  GunwOS
//
//  Created by Artur Danielewski on 12.03.2023.
//

#include <syscall/func.h>
#include <error/panic.h>

size_t k_scr_usr_debugPrint(const char * const buffer, const size_t bufferLen) {
    const ptr_t absBufferPtr = k_scl_func_getValidAbsoluteForCurrentProc((const ptr_t)buffer, bufferLen);
    if (!absBufferPtr) {
        OOPS("Null pointer referenced");
        return 0;
    }
    
    #warning TO BE IMPLEMENTED - MISSING CHARACTER OUTPUT
    return bufferLen;
}
