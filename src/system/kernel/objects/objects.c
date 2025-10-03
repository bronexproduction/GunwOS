//
//  objects.c
//  GunwOS
//
//  Created by Artur Danielewski on 01.10.2025.
//

#include "objects.h"
#include <defs.h>
#include <mem.h>
#include <hal/proc/proc.h>
#include <hal/criticalsec/criticalsec.h>

#define MAX_OBJECTS 16
#define MAX_OBJECT_DATA_SIZE_BYTES KiB(8)

static struct objectBox {
    procId_t owner;
    size_t sizeBytes;
    byte_t data[MAX_OBJECT_DATA_SIZE_BYTES];
} objects[MAX_OBJECTS];

static enum k_obj_error unsafe_reserveNextFreeBox(const size_t sizeBytes, k_obj_handle * const handle) {
    for (size_t i = 0; i < MAX_OBJECTS; ++i) {
        if (!objects[i].sizeBytes) {

            // That's suboptimal at best, only optimistic scenario covered

            CRITICAL_SECTION_BEGIN {
                if (objects[i].sizeBytes) {
                    CRITICAL_SECTION_END;
                    i = 0;
                    continue;
                }

                objects[i].sizeBytes = sizeBytes;
                CRITICAL_SECTION_END;
            }

            (*handle)=i;
            
            return OE_NONE;
        }
    }

    return OE_FULL;
}

static bool validateOwner(const procId_t owner) {
    return k_proc_isAlive(owner) || owner == KERNEL_PROC_ID;
}

static bool validateHandle(const k_obj_handle handle) {
    return handle < MAX_OBJECTS;
}

static void unsafe_clearBox(const k_obj_handle handle) {
    objects[handle].owner = NONE_PROC_ID;
    memzero(objects[handle].data, MAX_OBJECT_DATA_SIZE_BYTES);
    objects[handle].sizeBytes = 0;
}

static void unsafe_fillBox(const k_obj_handle handle,
                           const procId_t owner,
                           const void * const data) {
    objects[handle].owner = owner;
    memcopy(data, objects[handle].data, objects[handle].sizeBytes);
}

void k_obj_init() {
    for (size_t i = 0; i < MAX_OBJECTS; ++i) {
        unsafe_clearBox(i);
    }
}

enum k_obj_error k_obj_store(const procId_t owner,
                             const size_t sizeBytes,
                             const void * const data,
                             k_obj_handle * const handle) {
    if (!data) {
        return OE_INVALID_PARAMETER;        
    }
    if (!handle) {
        return OE_INVALID_PARAMETER;
    }
    if (!sizeBytes) {
        return OE_INVALID_PARAMETER;
    }
    if (sizeBytes > MAX_OBJECT_DATA_SIZE_BYTES) {
        return OE_SIZE_LIMIT_EXCEEDED;
    }
    if (!validateOwner(owner)) {
        return OE_INVALID_PARAMETER;
    }

    enum k_obj_error error = unsafe_reserveNextFreeBox(sizeBytes, handle);
    if (error != OE_NONE) {
        return error;
    }

    unsafe_fillBox(*handle, owner, data);

    return OE_NONE;
}

enum k_obj_error k_obj_retrieve(const procId_t owner,
                                const k_obj_handle handle,
                                const size_t sizeBytes,
                                data_t * const data) {
    if (!data) {
        return OE_INVALID_PARAMETER;
    }
    if (!validateHandle(handle)) {
        return OE_INVALID_PARAMETER;
    }
    if (objects[handle].owner != owner) {
        return OE_INVALID_PARAMETER;
    }
    if (objects[handle].sizeBytes != sizeBytes) {
        return OE_INVALID_PARAMETER;
    }

    memcopy(objects[handle].data, data, sizeBytes);

    return OE_NONE;
}
                                
enum k_obj_error k_obj_remove(const procId_t owner,
                              const k_obj_handle handle) {

    // TODO: what if the handle is 0? it's a valid index
    
    if (!validateHandle(handle)) {
        return OE_INVALID_PARAMETER;
    }
    if (objects[handle].owner != owner) {
        return OE_INVALID_PARAMETER;
    }

    unsafe_clearBox(handle);

    return OE_NONE;
}

void k_obj_procCleanup(const procId_t procId) {
    for (size_t i = 0; i < MAX_OBJECTS; ++i) {
        if (objects[i].owner == procId) {
            unsafe_clearBox(i);
        }
    }
}
