//
//  exec.cpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 21.07.2022.
//

#include "exec.hpp"

#include <chrono>
#include <thread>
#include <signal.h>

pid_t execShell(const std::string cmd);

pid_t spawnShell(const std::string cmd, const std::function<void()> childPreExecAction) {
    if (size_t pid = fork()) {
         // TODO: Avoiding returning valid PID on unknown command
        return pid;
    }

    childPreExecAction();
    return execShell(cmd);
}

pid_t execShell(const std::string cmd) {
    exit(execlp("bash", "sh", "-c", cmd.c_str(), NULL));
}

bool isPidUp(pid_t pid) {
    return !kill(pid, 0);
}
