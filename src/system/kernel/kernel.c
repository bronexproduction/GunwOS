//
//  kernel.c
//  GunwOS
//
//  Created by Artur Danielewski on 16.01.2018.
//

#include "../../../bootloader/preloader/kernel/kernel_data.h"

extern void k_tmr_init();
extern void k_hal_init();
extern void k_ipc_init();
extern void k_dev_init();
extern void k_log_init();
extern void k_drv_loadMinimal();
extern void k_stor_init();
extern void k_startup();
extern void k_que_start();

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

void __attribute__((fastcall, section(".start"))) __kernel_start(const struct k_krn_bootData * const bootData) {
    k_purge();

    k_tmr_init();
    k_hal_init();
    k_ipc_init();
    k_dev_init();
    k_log_init();
    k_drv_loadMinimal();
    k_stor_init();
    k_startup();
    k_que_start();
}
