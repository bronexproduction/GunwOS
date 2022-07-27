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
    if (!isNonBlocking(fd)) {
        throw std::runtime_error("Non-blocking descriptor expected");
    }

    pollfd fdDesc = {fd, POLLIN, 0};
    poll(&fdDesc, 1, timeoutMs);

    if (!(fdDesc.revents & POLLIN)) {
        throw std::runtime_error("Timeout waiting for data on descriptor");
    }
}

void waitfdstr(int fd, unsigned int timeoutMs, const std::string token) {
    if (!isNonBlocking(fd)) {
        throw std::runtime_error("Non-blocking descriptor expected");
    }

    std::chrono::system_clock::time_point deadline = std::chrono::system_clock::now();
    deadline += std::chrono::milliseconds(timeoutMs);

    while (std::chrono::system_clock::now() < deadline) {
        // read (od)bytes
    }

    throw std::runtime_error("Waiting for token timed out");
}

void flushfd(int fd) {
    if (!isNonBlocking(fd)) {
        throw std::runtime_error("Non-blocking descriptor expected");
    }

    char c;
    size_t totalBytes = 0;
    errno = 0;
    while (read(fd, &c, 1) > 0) {
        ++totalBytes;
    }

    if (errno != EAGAIN) {
        throw std::runtime_error("Error flushing descriptor");
    }
}

void writefd(int fd, const std::string string) {
    size_t length = string.length();
    if (write(fd, string.c_str(), length) != length) {
        throw std::runtime_error("Error writing string to descriptor");
    }
}
