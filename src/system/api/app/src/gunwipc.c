//
//  gunwipc.c
//  GunwOS
//
//  Created by Artur Danielewski on 08.05.2023.
//

#include "_gunwipc.h"
#include <mem.h>

#ifdef _GUNWAPI_KERNEL

void gnwIpcEndpointQuery_encode(const struct gnwIpcEndpointQuery * const absQueryPtr, ptr_t absDataPtr) {
    memcopy(absQueryPtr, absDataPtr, sizeof(struct gnwIpcEndpointQuery));
    size_t offset = sizeof(struct gnwIpcEndpointQuery);
    memcopy(absQueryPtr->params.dataPtr, absDataPtr + offset, absQueryPtr->params.dataSizeBytes);
    offset += absQueryPtr->params.dataSizeBytes;

    #warning result not handled
}

#else

void gnwIpcEndpointQuery_decode(const ptr_t absDataPtr, struct gnwIpcEndpointQuery * const absQueryPtr) {
    memcopy(absDataPtr, absQueryPtr, sizeof(struct gnwIpcEndpointQuery));
    absQueryPtr->params.dataPtr = absDataPtr + sizeof(struct gnwIpcEndpointQuery);

    #warning result not handled
}

#endif // _GUNWAPI_KERNEL
