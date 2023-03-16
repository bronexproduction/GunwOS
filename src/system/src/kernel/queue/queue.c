//
//  queue.c
//  GunwOS
//
//  Created by Artur Danielewski on 02.02.2021.
//

#include <types.h>
#include <defs.h>
#include <error/panic.h>
#include <schedule/schedule.h>
#include <hal/criticalsec/criticalsec.h>

#define MAX_QUEUE_LENGTH 64

typedef __attribute__((cdecl)) void (*fPtr_void)();
typedef __attribute__((cdecl)) void (*fPtr_32_32)(uint_32, uint_32);

union dispatchFuncPtr {
    fPtr_void f;
    fPtr_32_32 f_32_32;
};

union dispatchFuncParam {
    uint_32 p32;
    uint_16 p16;
    uint_8 p8;
};

enum dispatchFuncType {
    DFE_NONE = 0,
    DFE_VOID,
    DFE_32_32,
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
                     const uint_32 p0, 
                     const uint_32 p1) {
    
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
    case DFE_32_32:
        queue[i].func.ptr.f_32_32 = (fPtr_32_32)funcPtr;
        queue[i].func.params[0].p32 = (uint_32)p0;
        queue[i].func.params[1].p32 = (uint_32)p1; 
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

void k_que_dispatch(void (* const func)()) {
    dispatch((ptr_t)func, DFE_VOID, NULL, NULL);
}

void k_que_dispatch_32_32(void (* const func)(const uint_32, const uint_32), const uint_32 p0, const uint_32 p1) {
    dispatch((ptr_t)func, DFE_32_32, p0, p1);
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
        case DFE_32_32:
            enqueued->func.ptr.f_32_32(enqueued->func.params[0].p32, enqueued->func.params[1].p32);
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
