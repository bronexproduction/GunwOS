//
//  kernel.c
//  GunwOS
//
//  Created by Artur Danielewski on 16.01.2018.
//

#include "hal/hal.h"
#include "hal/mem/mem.h"
#include "timer/timer.h"
#include "ipc/ipc.h"
#include "dev/dev.h"
#include "log/log.h"
#include "driver/defaults.h"
#include "storage/storage.h"
#include "startup.h"
#include "queue/queue.h"

static void bss_clear() {
    extern int _BSS_START_, _BSS_END_;

    int *i = &_BSS_START_;
    while (i <= &_BSS_END_) {
        *(i++) = 0;
    }
}

PRIVATE void k_purge() {
    bss_clear();
}

void __attribute__((fastcall, section(".start"))) __kernel_start(const struct k_krn_bootData * const physBootData) {
    k_purge();

    __asm__ volatile ("jmp k_hal_prepare");
    __asm__ volatile ("__kernel_start_hal_prepare_end:");

    const struct k_krn_bootData * const bootData = (struct k_krn_bootData *)k_mem_physicalToLinear((ptr_t)physBootData);
    k_hal_init(bootData->memMap);
    k_tmr_init();
    k_ipc_init();
    k_dev_init();
    k_log_init();
    k_drv_loadMinimal();
    k_stor_init();
    k_startup();
    k_que_start();
}

__attribute__((naked)) void k_hal_prepare_end() {
    __asm__ volatile ("jmp __kernel_start_hal_prepare_end");
    __builtin_unreachable();
}
