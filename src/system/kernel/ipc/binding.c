//
//  binding.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.12.2023.
//

#include "binding.h"
#include "ipc.h"

#include <hal/proc/proc.h>
#include <error/panic.h>

#include <defs.h>
#include <mem.h>
#include <string.h>
#include <gunwfug.h>

struct binding {
    procId_t sender;
    procId_t receiver;
    size_t permissions;
} bindings[MAX_BINDING];

static void bindingClear(struct binding * const bindingPtr) {
    bindingPtr->sender = NONE_PROC_ID;
    bindingPtr->receiver = NONE_PROC_ID;
    bindingPtr->permissions = 0;
}

void k_ipc_binding_initialize() {
    for (size_t i = 0; i < MAX_BINDING; ++i) {
        bindingClear(&bindings[i]);
    }
}

static struct binding * find(const procId_t sender, const procId_t receiver) {
    for (size_t i = 0; i < MAX_BINDING; ++i) {
        if (bindings[i].sender == sender &&
            bindings[i].receiver == receiver) {
            return &bindings[i];
        }
    }

    return nullptr;
}

static struct binding * findFree() {
    return find(NONE_PROC_ID, NONE_PROC_ID);
}

enum gnwIpcError k_ipc_binding_create(const procId_t sender, const procId_t receiver, const size_t permissions) {
    struct binding * bindingSlotPtr = findFree();
    if (!bindingSlotPtr) {
        return GIPCE_FULL;
    }

    bindingSlotPtr->sender = sender;
    bindingSlotPtr->receiver = receiver;
    bindingSlotPtr->permissions = permissions;
    return GIPCE_NONE;
}

enum gnwIpcError k_ipc_binding_getPermissions(const procId_t sender, const procId_t receiver, size_t * const permissionsPtr) {
    CHECKPTR(permissionsPtr);
    struct binding * bindingPtr = find(sender, receiver);
    if (!bindingPtr) {
        return GIPCE_NOT_FOUND;
    }

    *permissionsPtr = bindingPtr->permissions;
    return GIPCE_NONE;
}

enum gnwIpcError k_ipc_binding_update(const procId_t sender, const procId_t receiver, const size_t permissions) {
    struct binding * bindingPtr = find(sender, receiver);
    if (!bindingPtr) {
        return GIPCE_NOT_FOUND;
    }

    bindingPtr->permissions = permissions;
    return GIPCE_NONE;
}

static void bindingDestroyNotify(const struct binding * const bindingPtr, const procId_t requester) {
    struct gnwIpcSenderQuery query;

    query.procId = NONE_PROC_ID;
    query.path = GNW_PATH_IPC_BINDING_NOTIFICATION_SESSION_DESTROYED;
    query.pathLen = strlen(GNW_PATH_IPC_BINDING_NOTIFICATION_SESSION_DESTROYED);
    query.dataPtr = (ptr_t)&requester;
    query.dataSizeBytes = sizeof(procId_t);
    query.replyErrPtr = nullptr;
    query.replyPtr = nullptr;
    query.replySizeBytes = 0;
    query.bindFlag = GIBF_NONE;
    query.permissions = 0;

    const enum gnwIpcError e = k_ipc_notify(query, bindingPtr->receiver == requester ? bindingPtr->sender : bindingPtr->receiver);
    if (e != GIPCE_NONE) {
        OOPS("Unexpected kernel event broadcast error");
        return;
    }
}

static void bindingDestroy(struct binding * const bindingPtr, const procId_t requester) {
    bindingDestroyNotify(bindingPtr, requester);
    bindingClear(bindingPtr);
}

enum gnwIpcError k_ipc_binding_destroy(const procId_t sender, const procId_t receiver, const procId_t requester) {
    struct binding * bindingPtr = find(sender, receiver);
    if (!bindingPtr) {
        return GIPCE_NOT_FOUND;
    }

    bindingDestroy(bindingPtr, requester);
    return GIPCE_NONE;
}

void k_ipc_binding_cleanup(const procId_t procId) {
    for (size_t i = 0; i < MAX_BINDING; ++i) {
        if (bindings[i].sender == procId) {
            bindingDestroy(&bindings[i], procId);
        } else if (bindings[i].receiver == procId) {
            bindingClear(&bindings[i]);
        }
    }
}
