//
//  spawntest.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.02.2023.
//

#include "../cmdutil.h"

extern void k_proc_spawn(void (*func)());
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

void cmd_spawntest(const char * const params) {
    CMD_NOPARAM

    k_proc_spawn(proc1);
    k_proc_spawn(proc2);
}
