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
//

#ifndef OPMODE_H
#define OPMODE_H

#define OPMODE_GRAP 0xF000

enum modeOfOperation {
                                        /* | Data mode | Colors (/ Palette) | Alpha format | Buffer start | Box size | Max pages (64K/128K/256K) | Resolution | */
    CD_OPMODE_0   = 0x00,               /* | Text      | 16                 | 40x25        | 0xB8000      | 8x8      | 8                         | 320x200    | */
    CD_OPMODE_1   = 0x01,               /* | Text      | 16                 | 40x25        | 0xB8000      | 8x8      | 8                         | 320x200    | */
    CD_OPMODE_2   = 0x02,               /* | Text      | 16                 | 80x25        | 0xB8000      | 8x8      | 8                         | 640x200    | */
    CD_OPMODE_3   = 0x03,               /* | Text      | 16                 | 80x25        | 0xB8000      | 8x8      | 8                         | 640x200    | */
    CD_OPMODE_4   = OPMODE_GRAP | 0x04, /* | Graphics  | 4                  | 40x25        | 0xB8000      | 8x8      | 1                         | 320x200    | */
    CD_OPMODE_5   = OPMODE_GRAP | 0x05, /* | Graphics  | 4                  | 40x25        | 0xB8000      | 8x8      | 1                         | 320x200    | */
    CD_OPMODE_6   = OPMODE_GRAP | 0x06, /* | Graphics  | 2                  | 80x25        | 0xB8000      | 8x8      | 1                         | 640x200    | */
    CD_OPMODE_D   = OPMODE_GRAP | 0x0D, /* | Graphics  | 16                 | 40x25        | 0xA0000      | 8x8      | 2/4/8                     | 320x200    | */
    CD_OPMODE_E   = OPMODE_GRAP | 0x0E, /* | Graphics  | 16                 | 80x25        | 0xA0000      | 8x8      | 1/2/4                     | 640x200    | */

    MD_OPMODE_7   = 0x07,               /* | Text      | 4                  | 80x25        | 0xB0000      | 9x14     | 8                         | 720x350    | */
    MD_OPMODE_F   = OPMODE_GRAP | 0x0F, /* | Graphics  | 4                  | 80x25        | 0xA0000      | 8x14     | 1/2                       | 640x350    | */

    ECD_OPMODE_0  = 0xF00,              /* | Text      | 16/64              | 40x25        | 0xB8000      | 8x14     | 8                         | 320x350    | */
    ECD_OPMODE_1  = 0xF01,              /* | Text      | 16/64              | 40x25        | 0xB8000      | 8x14     | 8                         | 320x350    | */
    ECD_OPMODE_2  = 0xF02,              /* | Text      | 16/64              | 80x25        | 0xB8000      | 8x14     | 8                         | 640x350    | */
    ECD_OPMODE_3  = 0xF03,              /* | Text      | 16/64              | 80x25        | 0xB8000      | 8x14     | 8                         | 640x350    | */
    ECD_OPMODE_10 = OPMODE_GRAP | 0xF10 /* | Graphics  | 4/16, 16/64        | 80x25        | 0xA0000      | 8x14     | 1/2                       | 640x350    | */
};

void setMode(const enum modeOfOperation mode);

#endif // OPMODE_H
