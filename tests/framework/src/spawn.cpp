//
//  spawn.cpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 21.07.2022.
//

#include "spawn.hpp"

pid_t spawnShell(const std::string cmd) {

    int status;
    if (size_t pid = fork()) {
        return pid;
    }
    exit(execlp("bash", "sh", "-c", cmd.c_str(), NULL));
}
