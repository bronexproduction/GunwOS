//
//  gunwdev.c
//  GunwOS
//
//  Created by Artur Danielewski on 08.05.2023.
//

#ifdef _GUNWAPI_KERNEL

#include "_gunwdev.h"
#include <mem.h>

void gnwDeviceEvent_serialize(const struct gnwDeviceEvent * const absEventPtr, ptr_t absDataPtr) {
    memcopy(absEventPtr, absDataPtr, sizeof(struct gnwDeviceEvent));
    memcopy(absEventPtr->data, absDataPtr + sizeof(struct gnwDeviceEvent), absEventPtr->dataSizeBytes);
}

void gnwDeviceEvent_deserialize(const ptr_t absDataPtr, struct gnwDeviceEvent * const absEventPtr) {
    memcopy(absDataPtr, absEventPtr, sizeof(struct gnwDeviceEvent));
    absEventPtr->data = absDataPtr + sizeof(struct gnwDeviceEvent);
}

#endif // _GUNWAPI_KERNEL
