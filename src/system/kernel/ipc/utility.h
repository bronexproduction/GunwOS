//
//  utility.h
//  GunwOS
//
//  Created by Artur Danielewski on 11.12.2023.
//

#ifndef UTILITY_H
#define UTILITY_H

#include <types.h>

bool k_ipc_utl_pathIsNotification(const char * absPathPtr, const size_t pathLen);
void k_ipc_utl_clearReply(const size_t entryId);
void k_ipc_utl_unlockIfAble(const procId_t procId);
size_t k_ipc_utl_nextListenerIndexForPath(const char * absPathPtr, const size_t pathLen, const size_t * const after);

#endif // UTILITY_H
