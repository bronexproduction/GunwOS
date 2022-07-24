//
//  exec.hpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 21.07.2022.
//

#ifndef EXEC_HPP
#define EXEC_HPP

#include <unistd.h>
#include <string>
#include <functional>

pid_t spawnShell(const std::string cmd, const std::function<void()> childPreExecAction = [](){});

#endif // EXEC_HPP