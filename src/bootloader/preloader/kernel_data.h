//
//  kernel_data.h
//  GunwOS Bootloader
//  Part of GunwOS project
//
//  Created by Artur Danielewski on 03.01.2021.
//

struct __attribute__((packed)) k_krn_bootData {

    char gdt_codeSegOffset;
    char gdt_dataSegOffset;
};