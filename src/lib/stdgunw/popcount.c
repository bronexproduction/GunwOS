//
//  popcount.c
//  GunwOS
//
//  Created by Artur Danielewski on 14.03.2023.
//
//  Copied from https://en.wikichip.org/wiki/population_count
//

int __popcountsi2(unsigned int a) {
    int c = 0;
    for (; a != 0; a &= a - 1)
        c++;
    return c;
}