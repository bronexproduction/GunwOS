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
    fPtr_void f;
    fPtr_arch_arch f_arch_arch;
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
    DFE_ARCH_ARCH,
};

struct dispatchFunc {
    enum dispatchFuncType type;
    union dispatchFuncPtr ptr;
    union dispatchFuncParam params[2];
};

struct dispatchEntry {
    volatile uint_8 reserved;
    struct dispatchFunc func;
    struct dispatchEntry *next;
};

static struct dispatchEntry queue[MAX_QUEUE_LENGTH];
struct dispatchEntry *k_que_currentDispatchEntry = 0;
static bool running = 0;

static void dispatch(const ptr_t funcPtr, 
                     const enum dispatchFuncType type, 
                     const union dispatchFuncParam p0, 
                     const union dispatchFuncParam p1) {
    
    #warning how to avoid duplicates?

    if (!running) {
        OOPS("Running queue required to dispatch items - aborting");
        return;
    }

    size_t i;
    
    for (i = 0; i < MAX_QUEUE_LENGTH; ++i) {
        if (!queue[i].reserved) {    
            queue[i].reserved = 1;
            break;
        }
    }
    
    if (i >= MAX_QUEUE_LENGTH) {
        OOPS("Run queue capacity exceeded");
        return;
    }

    queue[i].func.type = type;
    switch (type) {
    case DFE_VOID:
        queue[i].func.ptr.f = (fPtr_void)funcPtr;
        break;
    case DFE_ARCH_ARCH:
        queue[i].func.ptr.f_arch_arch = (fPtr_arch_arch)funcPtr;
        queue[i].func.params[0].pArch = p0.pArch;
        queue[i].func.params[1].pArch = p1.pArch; 
        break;
    default:
        OOPS("Unexpected dispatched function type");
        return;
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

void k_que_dispatch(const fPtr_void func) {
    dispatch((ptr_t)func, DFE_VOID, _NOPAR, _NOPAR);
}

void k_que_dispatch_arch_arch(const fPtr_arch_arch func, const addr_t p0, const addr_t p1) {
    dispatch((ptr_t)func, DFE_ARCH_ARCH, _PAR(p0), _PAR(p1));
}

void k_que_start() {
    running = true;
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
            OOPS("Enqueued item disabled");
            return;
        }
        if (!enqueued->func.ptr.f) {
            OOPS("Null pointer queued");
            return;
        }

        switch (enqueued->func.type) {
        case DFE_VOID:
            enqueued->func.ptr.f();
            break;
        case DFE_ARCH_ARCH:
            enqueued->func.ptr.f_arch_arch(enqueued->func.params[0].pArch, enqueued->func.params[1].pArch);
            break;
        default:
            OOPS("Unexpected queued function type");
            return;
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
