//
//  regset.hpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 25.07.2022.
//

#ifndef REGSET_HPP
#define REGSET_HPP

#include <cstdint>

struct i386_regSet {
    int16_t ax;
    int16_t bx;
    int16_t cx;
    int16_t dx;
};

#endif // REGSET_HPP
