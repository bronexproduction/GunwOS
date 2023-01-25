//
//  kernel.c
//  GunwOS
//
//  Created by Artur Danielewski on 16.01.2018.
//

#include "../../../../bootloader/preloader/kernel/kernel_data.h"

extern void k_tmr_init();
extern void k_hal_init(const char codeSegOffset);
extern void k_proc_init();
extern void k_dev_init();
extern void c_init();
extern void k_rlp_start();

static void bss_clear() {
    extern int _BSS_START_, _BSS_END_;

    int *i = &_BSS_START_;
    while (i <= &_BSS_END_) {
        *(i++) = 0;
    }
}

static void k_init() {
    bss_clear();

    // TODO: not sure where to put it (driver? service?)
    k_tmr_init();
}

extern void __attribute__((fastcall, section(".start"))) __kernel_start(const struct k_krn_bootData * const bootData);

void __attribute__((fastcall, section(".start"))) __kernel_start(const struct k_krn_bootData * const bootData) {
    k_init();

    k_hal_init(bootData->gdt_codeSegOffset);
    k_proc_init();
    k_dev_init();
    c_init();
    k_rlp_start();
}
