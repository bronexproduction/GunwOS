//
//  spawntest.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.02.2023.
//

#include "../cmdutil.h"
#include <stdgunw/mem.h>
#include <stdgunw/types.h>

extern void k_proc_spawn(ptr_t imageStart);
extern int c_trm_puts(const char * const s);

static void proc1() {
    while (1) {
        c_trm_puts("1");
    }
}

static void proc2() {
    while (1) {
        c_trm_puts("2");
    }
}

static void spawn(int index, ptr_t imageStart, size_t imageSize) {
    ptr_t loc = (ptr_t)(0x100000 + (0x100000 * index)); // 2 MB
    
    // Copy to another location (outsize of kernel space)
    memcopy(imageStart, loc, imageSize);

    k_proc_spawn(loc);
}

static bool alreadyDone = false;

void cmd_spawntest(const char * const params) {
    CMD_NOPARAM

    if (alreadyDone) {
        c_trm_puts("Unable to launch - already done");
        return;
    }

    alreadyDone = true;

    spawn(1, (ptr_t)proc1, 0x1000);
    spawn(2, (ptr_t)proc2, 0x1000);
}
