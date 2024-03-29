//
//  fd.hpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 25.07.2022.
//

#ifndef FD_HPP
#define FD_HPP

#include <string>

bool isNonBlocking(int fd);
void setNonBlocking(int fd);
int getFlags(int fd);
void setFlags(int fd, int flags);
void waitfd(int fd, unsigned int timeoutMs);
void waitfdstr(int fd, unsigned int timeoutMs, const std::string token);
void flushfd(int fd);
void writefd(int fd, const std::string string);

#endif // FD_HPP
