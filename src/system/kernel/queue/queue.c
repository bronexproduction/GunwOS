//
//  queue.c
//  GunwOS
//
//  Created by Artur Danielewski on 02.02.2021.
//

#include "queue.h"
#include <defs.h>
#include <error/panic.h>
#include <schedule/schedule.h>
#include <hal/criticalsec/criticalsec.h>

#define MAX_QUEUE_LENGTH 64

#define _PAR(VAL) ((union dispatchFuncParam)VAL)
#define _NOPAR _PAR((addr_t)NULL)

union dispatchFuncPtr {
    fPtr f;
    fPtr_arch f_arch;
    fPtr_arch2 f_arch2;
    fPtr_arch3 f_arch3;
};

union dispatchFuncParam {
    addr_t pArch;           /* Architecture dependent data size */
    int_32 p32;             /* 32-bit data */
    int_16 p16;             /* 16-bit data */
    int_8 p8;               /* 8-bit data */
};

enum dispatchFuncType {
    DFE_NONE = 0,
    DFE_VOID,
    DFE_ARCH,
    DFE_ARCH2,
    DFE_ARCH3,
};

struct dispatchFunc {
    enum dispatchFuncType type;
    union dispatchFuncPtr ptr;
    union dispatchFuncParam params[3];
};

struct dispatchEntry {
    volatile uint_8 reserved;
    struct dispatchFunc func;
    struct dispatchEntry *next;
};

static struct dispatchEntry queue[MAX_QUEUE_LENGTH];
struct dispatchEntry *k_que_currentDispatchEntry = 0;
PRIVATE bool queueRunning = 0;

static void dispatch(const ptr_t funcPtr,
                     const enum dispatchFuncType type, 
                     const union dispatchFuncParam p0, 
                     const union dispatchFuncParam p1, 
                     const union dispatchFuncParam p2) {
    
    #warning how to avoid duplicates?

    if (!queueRunning) {
        OOPS("Running queue required to dispatch items - aborting",);
    }

    size_t i;
    
    for (i = 0; i < MAX_QUEUE_LENGTH; ++i) {
        if (!queue[i].reserved) {    
            queue[i].reserved = 1;
            break;
        }
    }
    
    if (i >= MAX_QUEUE_LENGTH) {
        OOPS("Run queue capacity exceeded",);
    }

    queue[i].func.type = type;
    switch (type) {
    case DFE_VOID:
        queue[i].func.ptr.f = (fPtr)funcPtr;
        break;
    case DFE_ARCH:
        queue[i].func.ptr.f_arch = (fPtr_arch)funcPtr;
        queue[i].func.params[0].pArch = p0.pArch;
        break;
    case DFE_ARCH2:
        queue[i].func.ptr.f_arch2 = (fPtr_arch2)funcPtr;
        queue[i].func.params[0].pArch = p0.pArch;
        queue[i].func.params[1].pArch = p1.pArch;
        break;
    case DFE_ARCH3:
        queue[i].func.ptr.f_arch3 = (fPtr_arch3)funcPtr;
        queue[i].func.params[0].pArch = p0.pArch;
        queue[i].func.params[1].pArch = p1.pArch;
        queue[i].func.params[2].pArch = p2.pArch;
        break;
    default:
        OOPS("Unexpected dispatched function type",);
    }
    queue[i].next = 0;

    struct dispatchEntry * last = k_que_currentDispatchEntry;
    if (last) {
        while (last->next) {
            last = last->next;
        }
        last->next = (queue + i);
    }
    else {
        k_que_currentDispatchEntry = (queue + i);
    }
}

void k_que_dispatch(const fPtr func) {
    dispatch((ptr_t)func, DFE_VOID, _NOPAR, _NOPAR, _NOPAR);
}

void k_que_dispatch_arch(const fPtr_arch func, const addr_t p0) {
    dispatch((ptr_t)func, DFE_ARCH, _PAR(p0), _NOPAR, _NOPAR);
}

void k_que_dispatch_arch2(const fPtr_arch2 func, const addr_t p0, const addr_t p1) {
    dispatch((ptr_t)func, DFE_ARCH2, _PAR(p0), _PAR(p1), _NOPAR);
}

void k_que_dispatch_arch3(const fPtr_arch3 func, const addr_t p0, const addr_t p1, const addr_t p2) {
    dispatch((ptr_t)func, DFE_ARCH3, _PAR(p0), _PAR(p1), _PAR(p2));
}

void k_que_start() {
    queueRunning = true;
    while (1) {
        struct dispatchEntry *enqueued;
        CRITICAL_SECTION_BEGIN {
            enqueued = k_que_currentDispatchEntry;
            if (!enqueued) {
                k_proc_schedule_onKernelHandlingFinished();
                CRITICAL_SECTION_END;
                continue;
            }
            CRITICAL_SECTION_END;
        }
        
        if (!enqueued->reserved) {
            OOPS("Enqueued item disabled",);
        }
        if (!enqueued->func.ptr.f) {
            OOPS("Null pointer queued",);
        }

        switch (enqueued->func.type) {
        case DFE_VOID:
            enqueued->func.ptr.f();
            break;
        case DFE_ARCH:
            enqueued->func.ptr.f_arch(enqueued->func.params[0].pArch);
            break;
        case DFE_ARCH2:
            enqueued->func.ptr.f_arch2(enqueued->func.params[0].pArch,
                                       enqueued->func.params[1].pArch);
            break;
        case DFE_ARCH3:
            enqueued->func.ptr.f_arch3(enqueued->func.params[0].pArch,
                                       enqueued->func.params[1].pArch,
                                       enqueued->func.params[2].pArch);
            break;
        default:
            OOPS("Unexpected queued function type",);
        }

        CRITICAL_SECTION (
            struct dispatchEntry * const next = enqueued->next;
            k_que_currentDispatchEntry = next;
            enqueued->next = 0;
            enqueued->func.type = DFE_NONE;
            enqueued->reserved = 0;
        )
    }
}
