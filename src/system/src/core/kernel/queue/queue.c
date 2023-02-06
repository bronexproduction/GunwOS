//
//  queue.c
//  GunwOS
//
//  Created by Artur Danielewski on 02.02.2021.
//

#include <stdgunw/types.h>
#include <stdgunw/defs.h>
#include "../../log/log.h"
#include "../proc/schedule.h"
#include "../common/criticalsec.h"

#define MAX_QUEUE_LENGTH 64

struct dispatchEntry {
    volatile uint_8 reserved;
    void (*func)();
    struct dispatchEntry *next;
};

static struct dispatchEntry queue[MAX_QUEUE_LENGTH];
static struct dispatchEntry *current = 0;
static ptr_t sPtr = 0;

void k_que_dispatch(void (* const func)()) {
    if (!sPtr) {
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
        LOG_FATAL("Run queue capacity exceeded");
        return;
    }

    queue[i].func = func;
    queue[i].next = 0;

    struct dispatchEntry * last = current;
    if (last) {
        while (last->next) {
            last = last->next;
        }
        last->next = (queue + i);
    }
    else {
        current = (queue + i);
    }

    k_proc_schedule_intNeedsKernelHandling();
}

void k_que_start() {
    #warning SAVE THE QUEUE STACK POINTER (sPtr)
    #warning how to avoid duplicates?
    while (1) {
        struct dispatchEntry *enqueued;
        CRITICAL_SECTION_BEGIN {
            enqueued = current;
            if (!enqueued) {
                k_proc_schedule_onKernelHandlingFinished();
                continue;
            }
            else {
                CRITICAL_SECTION_END;
            }
        }
        
        if (!enqueued->reserved) {
            LOG_FATAL("Enqueued item disabled");
            return;
        }
        if (!enqueued->func) {
            LOG_FATAL("Null pointer queued");
            return;
        }

        enqueued->func();

        CRITICAL_SECTION (
            struct dispatchEntry * const next = enqueued->next;
            current = next;
            enqueued->next = 0;
            enqueued->func = nullptr;
            enqueued->reserved = 0;
        )
    }
}
