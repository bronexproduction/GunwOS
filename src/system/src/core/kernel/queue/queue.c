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

#define MAX_QUEUE_LENGTH 255

struct dispatchEntry {
    volatile uint_8 reserved;
    void (*func)();
    struct dispatchEntry *next;
};

static struct dispatchEntry queue[MAX_QUEUE_LENGTH];
static struct dispatchEntry *current = 0;

void k_que_dispatch(void (* const func)()) {
    
    size_t i;
    for (i = 0; i < MAX_QUEUE_LENGTH; ++i) {
        if (!queue[i].reserved) {
            queue[i].reserved = 1;
            if (queue[i].func) {
                
                // some higher-priority code already reserved this slot
                // after we checked the reservation but before we set reserved to 1

                // retry looking for free slot
                i = 0;

                continue;
            }
            queue[i].func = func;
            queue[i].next = 0;
            break;
        }
    }

    if (i >= MAX_QUEUE_LENGTH) {
        LOG_FATAL("Run queue capacity exceeded");
        return;
    }

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

    k_proc_schedule_setNeedsKernelHandling();
}

void k_que_start() {
    
    while (1) {

        struct dispatchEntry * const enqueued = current;
        if (enqueued) {
            if (!enqueued->reserved) {
                LOG_FATAL("Enqueued item disabled");
                return;
            }
            if (enqueued->func) {
                enqueued->func();
            }
            else {
                LOG_FATAL("Null pointer queued");
                return;
            }

            struct dispatchEntry * const next = enqueued->next;
            current = next;
            enqueued->next = 0;
            enqueued->func = nullptr;
            enqueued->reserved = 0;
        }
    }
}
