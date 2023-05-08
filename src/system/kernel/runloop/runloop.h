//
//  runloop.h
//  GunwOS
//
//  Created by Artur Danielewski on 08.05.2023.
//

#ifndef RUNLOOP_H
#define RUNLOOP_H

#include <src/_gunwrlp.h>

enum gnwRunLoopError k_runloop_reserve(const procId_t procId, size_t * const token);
enum gnwRunLoopError k_runloop_dispatch(const size_t token, const struct gnwRunLoopDispatchItem item, const ptr_t data);
enum gnwRunLoopError k_runloop_getPendingItem(const procId_t procId, struct gnwRunLoopDispatchItem * const absItemPtr);
enum gnwRunLoopError k_runloop_getPendingItemDataSizeBytes(const procId_t procId, size_t * const dataSizeBytes);
enum gnwRunLoopError k_runloop_getPendingItemData(const procId_t procId, ptr_t absDataBufferPtr);
bool k_runloop_isEmpty(const procId_t procId);
void k_runloop_procCleanup(const procId_t procId);

#endif // RUNLOOP_H
