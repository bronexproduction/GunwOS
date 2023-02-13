//
//  gdt.h
//  GunwOS
//
//  Created by Artur Danielewski on 13.02.2023.
//

#ifndef GDT_H
#define GDT_H



enum k_gdt_dpl {
    DPL_0 = 0b00,
    DPL_1 = 0b01,
    DPL_2 = 0b10,
    DPL_3 = 0b11
};

struct __attribute__((packed)) k_gdt_codeEntry {
    uint_16 limitL              :16;
    uint_32 baseL               :24;
    uint_8 accessed             :1;
    uint_8 readable             :1;
    uint_8 conforming           :1;
    const uint_8 _43_1          :1;
    const uint_8 _44_1          :1;
    enum k_gdt_dpl privilege    :2;
    uint_8 present              :1;
    uint_8 limitH               :4;
    uint_8 available            :1;
    const uint_8 _53_0          :1;
    uint_8 defOperandSize       :1;         // 1 - 32-bit, 0 - 16-bit
    uint_8 granularity          :1;         // 1 - 4K, 0 - 1 byte
    uint_8 baseH                :8;
};

struct __attribute__((packed)) k_gdt_dataEntry {
    uint_16 limitL              :16;
    uint_32 baseL               :24;
    uint_8 accessed             :1;
    uint_8 writeable            :1;
    uint_8 expansionDirection   :1;
    const uint_8 _43_0          :1;
    const uint_8 _44_1          :1;
    enum k_gdt_dpl privilege    :2;
    uint_8 present              :1;
    uint_8 limitH               :4;
    uint_8 available            :1;
    const uint_8 _53_0          :1;
    uint_8 big                  :1;         // 1 - 32-bit, 0 - 16-bit
    uint_8 granularity          :1;         // 1 - 4K, 0 - 1 byte
    uint_8 baseH                :8;
};

struct __attribute__((packed)) k_gdt_tssEntry {
    uint_16 limitL              :16;
    uint_32 baseL               :24;
    const uint_8 _40_1          :1;
    uint_8 busy                 :1;
    const uint_8 _42_0          :1;
    uint_8 big                  :1;         // 1 - 32-bit, 0 - 16-bit
    const uint_8 _44_0          :1;
    const uint_8 _45_0          :1;
    const uint_8 _46_0          :1;
    uint_8 present              :1;
    uint_8 limitH               :4;
    const uint_8 _52_0          :1;
    const uint_8 _53_0          :1;
    const uint_8 _54_0          :1;
    uint_8 granularity          :1;         // 1 - 4K, 0 - 1 byte
    uint_8 baseH                :8;
};

struct __attribute__((packed)) k_gdt_gdt {
    const uint_64 _null;
    struct k_gdt_codeEntry r0code;
    struct k_gdt_dataEntry r0data;
    struct k_gdt_codeEntry r3code;
    struct k_gdt_dataEntry r3data;
    struct k_gdt_tssEntry tss;
};

extern const struct k_gdt_gdt k_gdt_gdt;

void k_gdt_loadDefault();

#endif // GDT_H
