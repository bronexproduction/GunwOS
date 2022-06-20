//
//  boot_data.h
//  GunwOS Bootloader
//  Part of GunwOS project
//
//  Created by Artur Danielewski on 03.01.2021.
//

struct __attribute__((packed)) k_pld_bootData {

    // This data structure should correspond
    // with data layout specified in preloader_data.s

    char boot_drive;
};