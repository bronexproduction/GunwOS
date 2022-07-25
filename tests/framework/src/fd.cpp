//
//  fd.cpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 25.07.2022.
//

#include "fd.hpp"

#include <unistd.h>
#include <cstdio>
#include <stdexcept>

void flushfd(int fd) {
    char c;
    int status;
    do {
        status = read(fd, &c, 1);
        if (status < 0) {
            throw std::runtime_error("FD input read failure");
        }
        if (status == 0) {
            printf("Status je zero\n");
        }
        if (status > 0) {
            printf("Status wiekszy od zero\n");
        }
    } while (status > 0);
}
