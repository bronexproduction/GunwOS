//
//  binding.h
//  GunwOS
//
//  Created by Artur Danielewski on 11.12.2023.
//

#ifndef BINDING_H
#define BINDING_H

#include <types.h>
#include <gunwipc.h>

#define MAX_BINDING 16

#warning that may be not particularly efficient solution not to use any binding identifiers

void k_ipc_binding_initialize();

enum gnwIpcError k_ipc_binding_create(const procId_t sender, const procId_t receiver, const size_t permissions);
enum gnwIpcError k_ipc_binding_getPermissions(const procId_t sender, const procId_t receiver, size_t * const permissionsPtr);
enum gnwIpcError k_ipc_binding_update(const procId_t sender, const procId_t receiver, const size_t permissions);
enum gnwIpcError k_ipc_binding_destroy(const procId_t sender, const procId_t receiver, const procId_t requester);
void k_ipc_binding_cleanup(const procId_t procId);

#endif // BINDING_H
