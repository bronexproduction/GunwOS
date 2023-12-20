//
//  utility.h
//  GunwOS
//
//  Created by Artur Danielewski on 11.12.2023.
//

#ifndef UTILITY_H
#define UTILITY_H

#include <types.h>

bool k_ipc_utl_pathGlobalValidate(const data_t absPathData);
bool k_ipc_utl_pathDirectValidate(data_t absPathData);
bool k_ipc_utl_pathNotificationValidate(data_t absPathData);
void k_ipc_utl_clearReply(const size_t entryId);
void k_ipc_utl_unlockIfAble(const procId_t procId);
size_t k_ipc_utl_nextListenerIndexForPath(const data_t absPathData, const size_t * const after);

#endif // UTILITY_H
