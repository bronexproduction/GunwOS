//
//  opmode.h
//  GunwOS
//
//  Created by Artur Danielewski on 26.03.2023.
//
//  For more information see:
//  IBM Enhanced Graphics Adapter
//  Section "Modes of operation"
//  page 5+
//  and
//  Intel® OpenSource HD Graphics Programmer’s Reference Manual (PRM)
//

#ifndef OPMODE_H
#define OPMODE_H

#warning buffer start addresses to be verified

enum modeOfOperation {                  /* | Data mode | Colors         | Alpha format | Buffer start | Box size | Resolution | */
    
    VGA_OPMODE_0  = 0x00,               /* | Text      | 16 (greyscale) | 40x25        | 0xB8000 ?    | 9x16     | 360x400    | */
    VGA_OPMODE_1  = 0x01,               /* | Text      | 16             | 40x25        | 0xB8000 ?    | 9x16     | 360x400    | */
    VGA_OPMODE_2  = 0x02,               /* | Text      | 16 (greyscale) | 80x25        | 0xB8000 ?    | 9x16     | 720x400    | */
    VGA_OPMODE_3  = 0x03,               /* | Text      | 16             | 80x25        | 0xB0000 ?    | 9x16     | 720x400    | */
    VGA_OPMODE_4  = 0x04,               /* | Graphics  | 4              | 40x25        | 0xB8000 ?    | 8x8      | 320x200    | */
    VGA_OPMODE_5  = 0x05,               /* | Graphics  | 4 (greyscale)  | 40x25        | 0xB8000 ?    | 8x8      | 320x200    | */
    VGA_OPMODE_6  = 0x06,               /* | Graphics  | 2              | 80x25        | 0xB8000 ?    | 8x8      | 640x200    | */
    VGA_OPMODE_D  = 0x0D,               /* | Graphics  | 16             | 40x25        | 0xA0000 ?    | 8x8      | 320x200    | */
    VGA_OPMODE_E  = 0x0E,               /* | Graphics  | 16             | 80x25        | 0xA0000 ?    | 8x8      | 640x200    | */
    VGA_OPMODE_F  = 0x0F,               /* | Graphics  | 2              | 80x25        | 0xA0000 ?    | 8x14     | 640x350    | */
    VGA_OPMODE_10 = 0x10,               /* | Graphics  | 16             | 80x25        | 0xA0000 ?    | 8x14     | 640x350    | */
    VGA_OPMODE_11 = 0x11,               /* | Graphics  | 2              | 80x30        | 0xB8000 ?    | 8x16     | 640x480    | */
    VGA_OPMODE_12 = 0x12,               /* | Graphics  | 16             | 80x30        | 0xB8000 ?    | 8x16     | 640x480    | */
    VGA_OPMODE_13 = 0x13,               /* | Graphics  | 256            | 40x25        | 0xB8000 ?    | 8x8      | 320x200    | */
};

void setMode(const enum modeOfOperation mode);

#endif // OPMODE_H
