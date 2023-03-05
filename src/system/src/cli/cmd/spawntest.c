//
//  spawntest.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.02.2023.
//

#include "../cmdutil.h"
#include <stdgunw/mem.h>
#include <stdgunw/types.h>
#include <gunwio.h>
#include "../../core/kernel/hal/proc/proc.h"
#include "../../core/kernel/hal/mem/mem.h"

extern int c_trm_puts(const char * const s);

static void proc1() {
    while (1) {
        debugPrint("1");
    }
}

static void proc2() {
    while (1) {
        debugPrint("2");
    }
}

static void spawn(int index, ptr_t imageStart, size_t imageSize) {
    /*
        Each process takes 2 MB of space, starting from MEM_KERNEL_START + MEM_KERNEL_RESERVED_BYTES
    */
    ptr_t img = (ptr_t)(MEM_KERNEL_START + MEM_KERNEL_RESERVED_BYTES + MiB(index * 2 + 1));
    
    // Copy to another location (outsize of kernel space)
    memcopy(imageStart, img, imageSize);

    struct k_proc_descriptor desc;

    desc.img = img;
    desc.imgBytes = imageSize;

    k_proc_spawn(&desc);
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
