//
//  gdt.h
//  GunwOS
//
//  Created by Artur Danielewski on 13.02.2023.
//

#ifndef GDT_H
#define GDT_H

#define GDT_OFFSET(FIELD) ((ptr_t)&k_gdt_gdt.FIELD - (ptr_t)&k_gdt_gdt)
#define GDT_LIMIT_BYTES(FIELD) ((((size_t)k_gdt_gdt.FIELD.limitH) << 16 | (size_t)k_gdt_gdt.FIELD.limitL) * ((k_gdt_gdt.FIELD.granularity == GRAN_4K) ? KiB(4) : 1))
#define GDT_SEGMENT_START(FIELD) ((ptr_t)(((size_t)k_gdt_gdt.FIELD.baseH) << 24 | (size_t)k_gdt_gdt.FIELD.baseL))
#define GDT_SEGMENT_END(FIELD) ((ptr_t)(GDT_SEGMENT_START(FIELD) + GDT_LIMIT_BYTES(FIELD) - 1))

enum k_gdt_dpl {
    DPL_0 = 0b00,
    DPL_1 = 0b01,
    DPL_2 = 0b10,
    DPL_3 = 0b11
};

enum k_gdt_granularity {
    GRAN_BYTE   = 0,
    GRAN_4K     = 1
};

enum k_gdt_opSize {
    OPS_16BIT   = 0,
    OPS_32BIT   = 1
};

enum k_gdt_expDir {
    EXD_UP      = 0,
    EXD_DOWN    = 1
};

struct __attribute__((packed)) k_gdt_codeEntry {
    uint_16 limitL                      :16;
    uint_32 baseL                       :24;
    bool accessed                       :1;
    bool readable                       :1;
    bool conforming                     :1;
    const uint_8 _43_1                  :1;
    const uint_8 _44_1                  :1;
    enum k_gdt_dpl privilege            :2;
    bool present                        :1;
    uint_8 limitH                       :4;
    bool available                      :1;
    const uint_8 _53_0                  :1;
    enum k_gdt_opSize defOperandSize    :1;
    enum k_gdt_granularity granularity  :1;
    uint_8 baseH                        :8;
};

struct __attribute__((packed)) k_gdt_dataEntry {
    uint_16 limitL                      :16;
    uint_32 baseL                       :24;
    bool accessed                       :1;
    bool writeable                      :1;
    enum k_gdt_expDir expandDirection   :1;
    const uint_8 _43_0                  :1;
    const uint_8 _44_1                  :1;
    enum k_gdt_dpl privilege            :2;
    bool present                        :1;
    uint_8 limitH                       :4;
    bool available                      :1;
    const uint_8 _53_0                  :1;
    bool big                            :1;         // 1 - 32-bit, 0 - 16-bit
    enum k_gdt_granularity granularity  :1;
    uint_8 baseH                        :8;
};

struct __attribute__((packed)) k_gdt_tssEntry {
    uint_16 limitL                      :16;
    uint_32 baseL                       :24;
    const uint_8 _40_1                  :1;
    bool busy                           :1;
    const uint_8 _42_0                  :1;
    const uint_8 _43_1                  :1;
    const uint_8 _44_0                  :1;
    const uint_8 _45_0                  :1;
    const uint_8 _46_0                  :1;
    bool present                        :1;
    uint_8 limitH                       :4;
    const uint_8 _52_0                  :1;
    const uint_8 _53_0                  :1;
    const uint_8 _54_0                  :1;
    enum k_gdt_granularity granularity  :1;
    uint_8 baseH                        :8;
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

void k_gdt_init();

#endif // GDT_H
