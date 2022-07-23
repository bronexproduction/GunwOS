//
//  exec.cpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 21.07.2022.
//

#include "exec.hpp"

pid_t spawnShell(const std::string cmd, const std::function<void()> childPreExecAction) {
    if (size_t pid = fork()) {
        return pid;
    }
    childPreExecAction();
    return execShell(cmd);
}

pid_t execShell(const std::string cmd) {
    exit(execlp("bash", "sh", "-c", cmd.c_str(), NULL));
}
