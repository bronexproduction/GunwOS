//
//  queue.c
//  GunwOS
//
//  Created by Artur Danielewski on 02.02.2021.
//

#include <stdgunw/types.h>
#include <stdgunw/defs.h>
#include "../../log/log.h"
#include "../schedule/schedule.h"
#include "../hal/criticalsec/criticalsec.h"

#define MAX_QUEUE_LENGTH 64

struct dispatchEntry {
    volatile uint_8 reserved;
    void (*func)();
    struct dispatchEntry *next;
};

static struct dispatchEntry queue[MAX_QUEUE_LENGTH];
struct dispatchEntry *k_que_currentDispatchEntry = 0;
static bool running = 0;

void k_que_dispatch(void (* const func)()) {

    #warning how to avoid duplicates?

    if (!running) {
        LOG_FATAL("Running queue required to dispatch items - aborting");
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
            k_que_currentDispatchEntry = next;
            enqueued->next = 0;
            enqueued->func = nullptr;
            enqueued->reserved = 0;
        )
    }
}
