//
//  spawn.hpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 21.07.2022.
//

#ifndef SPAWN_HPP
#define SPAWN_HPP

#include <unistd.h>
#include <string>

pid_t spawnShell(const std::string cmd);

#endif // SPAWN_HPP