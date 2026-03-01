//
//  gunwnetwork.c
//  GunwOS
//
//  Created by Artur Danielewski on 27.02.2026.
//

#ifndef _GUNWAPI_KERNEL

#include "../include/gunwnetwork.h"
#include <mem.h>
#include "../include/gunwfug.h"
#include "../include/gunwipc.h"
#include <netmgr.h>

enum gnwNetworkError getMac(gnwNetworkMacAddr * const result) {
    CHECKPTR(result);

    *result = (gnwNetworkMacAddr){
        .bytes = {1, 2, 4, 8, 16, 32}
    };

    return GNE_NONE;
}

enum gnwNetworkError icmpEcho() {

    // CHECKPTR(displayDescriptor)
    
    // struct gnwDisplayManagerGetDisplayQuery query;
    // query.type = type;
    // struct gnwDisplayManagerGetDisplayResult result;

    // enum gnwIpcError error = ipcSend(DISPMGR_PATH_GET,
    //                                  (data_t){ (ptr_t)&query, sizeof(struct gnwDisplayManagerGetDisplayQuery) },
    //                                  (data_t){ (ptr_t)&result, sizeof(struct gnwDisplayManagerGetDisplayResult) },
    //                                  (struct gnwIpcBindData){ GIBF_NONE, 0 });
    // if (error != GIPCE_NONE) {
    //     return GDE_OPERATION_FAILED;
    // } else if (result.error != GDE_NONE) {
    //     return result.error;
    // }
    
    // memcopy(&result.displayDescriptor, displayDescriptor, sizeof(struct gnwDisplayDescriptor));
    
    return GNE_NONE;
}

#endif // _GUNWAPI_KERNEL
