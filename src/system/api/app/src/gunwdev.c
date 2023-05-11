//
//  gunwdev.c
//  GunwOS
//
//  Created by Artur Danielewski on 08.05.2023.
//

#include "_gunwdev.h"
#include <mem.h>

#ifdef _GUNWAPI_KERNEL

void gnwDeviceEvent_encode(const struct gnwDeviceEvent * const absEventPtr, ptr_t absDataPtr) {
    memcopy(absEventPtr, absDataPtr, sizeof(struct gnwDeviceEvent));
    memcopy(absEventPtr->data, absDataPtr + sizeof(struct gnwDeviceEvent), absEventPtr->dataSizeBytes);
}

#else

void gnwDeviceEvent_decode(const ptr_t absDataPtr, struct gnwDeviceEvent * const absEventPtr) {
    memcopy(absDataPtr, absEventPtr, sizeof(struct gnwDeviceEvent));
    absEventPtr->data = absDataPtr + sizeof(struct gnwDeviceEvent);
}

#endif // _GUNWAPI_KERNEL
