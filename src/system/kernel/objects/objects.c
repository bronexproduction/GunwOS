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

static enum k_obj_error unsafe_reserveNextFreeBox(const size_t sizeBytes, size_t * const id) {
    for (size_t i = 0; i < MAX_OBJECTS; ++i) {
        if (!objects[i].sizeBytes) {

            // That's suboptimal at best, only optimistic scenario covered

            CRITICAL_SECTION_BEGIN {
                if (objects[i].sizeBytes) {
                    CRITICAL_SECTION_END;
                    continue;
                }

                objects[i].sizeBytes = sizeBytes;
                CRITICAL_SECTION_END;
            }

            (*id)=i;
            
            return OE_NONE;
        }
    }

    return OE_FULL;
}

static bool validateOwner(const procId_t owner) {
    return k_proc_isAlive(owner) || owner == KERNEL_PROC_ID;
}

static bool validateId(const size_t id) {
    return id < MAX_OBJECTS;
}

static void unsafe_clearBox(const size_t id) {
    objects[id].owner = NONE_PROC_ID;
    memzero(objects[id].data, MAX_OBJECT_DATA_SIZE_BYTES);
    objects[id].sizeBytes = 0;
}

static void unsafe_fillBox(const size_t id,
                           const procId_t owner,
                           const size_t sizeBytes,
                           const void * const data) {
    objects[id].owner = owner;
    objects[id].sizeBytes = sizeBytes;
    memcopy(data, objects[id].data, sizeBytes);
}

void k_obj_init() {
    for (size_t i = 0; i < MAX_OBJECTS; ++i) {
        unsafe_clearBox(i);
    }
}

enum k_obj_error k_obj_store(const procId_t owner,
                             const size_t sizeBytes,
                             const void * const data,
                             size_t * const id) {
    if (!data) {
        return OE_INVALID_PARAMETER;        
    }
    if (!id) {
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

    enum k_obj_error error = unsafe_reserveNextFreeBox(sizeBytes, id);
    if (error != OE_NONE) {
        return error;
    }

    unsafe_fillBox(*id, owner, sizeBytes, data);

    return OE_NONE;
}

enum k_obj_error k_obj_retrieve(const procId_t owner,
                                const size_t id,
                                const size_t sizeBytes,
                                data_t * const data) {
    if (!data) {
        return OE_INVALID_PARAMETER;
    }
    if (!validateId(id)) {
        return OE_INVALID_PARAMETER;
    }
    if (objects[id].owner != owner) {
        return OE_INVALID_PARAMETER;
    }
    if (objects[id].sizeBytes != sizeBytes) {
        return OE_INVALID_PARAMETER;
    }

    memcopy(objects[id].data, data, sizeBytes);

    return OE_NONE;
}
                                
enum k_obj_error k_obj_remove(const procId_t owner,
                              const size_t id) {
    if (!validateId(id)) {
        return OE_INVALID_PARAMETER;
    }
    if (objects[id].owner != owner) {
        return OE_INVALID_PARAMETER;
    }

    unsafe_clearBox(id);

    return OE_NONE;
}
