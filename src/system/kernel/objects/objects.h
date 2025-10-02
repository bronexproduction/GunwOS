//
//  objects.h
//  GunwOS
//
//  Created by Artur Danielewski on 01.10.2025.
//

#ifndef OBJECTS_H
#define OBJECTS_H

#include <types.h>

typedef size_t k_obj_handle;

enum k_obj_error {
    OE_NONE = 0,
    OE_NOT_FOUND,
    OE_INVALID_PARAMETER,
    OE_SIZE_LIMIT_EXCEEDED,
    OE_FULL
};

void k_obj_init();

enum k_obj_error k_obj_store(const procId_t owner,
                             const size_t sizeBytes,
                             const void * const data,
                             k_obj_handle * const handle);

enum k_obj_error k_obj_retrieve(const procId_t owner,
                                const k_obj_handle handle,
                                const size_t sizeBytes,
                                data_t * const data);

enum k_obj_error k_obj_remove(const procId_t owner,
                              const k_obj_handle handle);

void k_obj_procCleanup(const procId_t procId);

#endif // OBJECTS_H
