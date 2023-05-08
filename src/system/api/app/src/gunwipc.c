//
//  gunwipc.c
//  GunwOS
//
//  Created by Artur Danielewski on 08.05.2023.
//

#ifdef _GUNWAPI_KERNEL

void gnwIpcEndpointQuery_serialize(const struct gnwIpcEndpointQuery * const absEventPtr, ptr_t absDataPtr) {
    #warning TO BE IMPLEMENTED
    // memcopy(absEventPtr, absDataPtr, sizeof(struct gnwDeviceEvent));
    // memcopy(absEventPtr->data, absDataPtr + sizeof(struct gnwDeviceEvent), absDataPtr->dataSizeBytes);
}

void gnwIpcEndpointQuery_deserialize(const ptr_t absDataPtr, struct gnwIpcEndpointQuery * const absEventPtr) {
    #warning TO BE IMPLEMENTED
    // memcopy(absDataPtr, absEventPtr, sizeof(struct gnwDeviceEvent));
    // absEventPtr.data = absDataPtr + sizeof(struct gnwDeviceEvent);
}

#endif // _GUNWAPI_KERNEL
