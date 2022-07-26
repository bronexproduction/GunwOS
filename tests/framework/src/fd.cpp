//
//  fd.cpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 25.07.2022.
//

#include "fd.hpp"

#include <unistd.h>
#include <stdexcept>
#include <fcntl.h>
#include <poll.h>

bool isNonBlocking(int fd) {
    return fcntl(fd, F_GETFL) & O_NONBLOCK;
}

void setNonBlocking(int fd) {
    int flags = fcntl(fd, F_GETFL);
    flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags)) {
        throw std::runtime_error("Unable to set descriptor flags");
    }
}

void waitfd(int fd, unsigned int timeoutMs) {
    pollfd fdDesc = {fd, POLLIN, 0};
    poll(&fdDesc, 1, timeoutMs);

    if (!(fdDesc.revents & POLLIN)) {
        throw std::runtime_error("Timeout waiting for data on descriptor");
    }
}

void flushfd(int fd) {
    char c;
    size_t totalBytes = 0;
    errno = 0;
    while (read(fd, &c, 1) > 0) {
        ++totalBytes;
    }

    if (errno) {
        if (errno != EAGAIN || !isNonBlocking(fd)) {
            printf("%d\n", errno);  
            throw std::runtime_error("Error flushing descriptor");
        }
    }
}

void writefd(int fd, const std::string string) {
    size_t length = string.length();
    if (write(fd, string.c_str(), length) != length) {
        throw std::runtime_error("Error writing string to descriptor");
    }
}
