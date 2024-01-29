//
//  shellexecutable.cpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 29.07.2022.
//

#include "shellexecutable.hpp"

#include <unistd.h>
#include <sstream>
#include <signal.h>

#include "exec.hpp"
#include "fd.hpp"

class ShellExecutablePrivate {

    private:
        ~ShellExecutablePrivate();

    private:
        pid_t pid = -1;
        int inFd = -1;
        int outFd = -1;

    friend class ShellExecutable;
};

ShellExecutable::ShellExecutable(): d(new ShellExecutablePrivate()) {}
ShellExecutable::~ShellExecutable() { 
    delete d;
}

void ShellExecutable::Launch() {
    int inputPipe[2];
    int outputPipe[2];

    if (pipe(inputPipe)) {
        throw std::runtime_error("Failed to create input pipe");
    }
    if (pipe(outputPipe)) {
        throw std::runtime_error("Failed to create output pipe");
    }
    
    const std::string command = BuildCommand();
    d->pid = spawnShell(command, [=](){
        int flags = getFlags(STDIN_FILENO);
        if (close(STDIN_FILENO)) {                      throw std::runtime_error("Unable to close stdin"); }
        if (dup2(inputPipe[0], STDIN_FILENO) < 0) {     throw std::runtime_error("Unable to duplicate input pipe descriptor"); }
        setFlags(STDIN_FILENO, flags);
        if (close(inputPipe[0])) {                      throw std::runtime_error("Unable to close input pipe read side"); }
        if (close(inputPipe[1])) {                      throw std::runtime_error("Unable to close input pipe write side"); }
        flags = getFlags(STDOUT_FILENO);
        if (close(STDOUT_FILENO)) {                     throw std::runtime_error("Unable to close stdout"); }
        if (dup2(outputPipe[1], STDOUT_FILENO) < 0) {   throw std::runtime_error("Unable to duplicate output pipe descriptor"); }
        setFlags(STDOUT_FILENO, flags);
        if (close(outputPipe[0])) {                     throw std::runtime_error("Unable to close output pipe read side"); }
        if (close(outputPipe[1])) {                     throw std::runtime_error("Unable to close output pipe write side"); }
    });
    
    if (d->pid < 0) {
        std::stringstream ss;
        ss << "Unable to launch: " << command;
        throw std::runtime_error(ss.str());
    }

    if (close(inputPipe[0])) {  throw std::runtime_error("Unable to close input pipe read side"); }
    if (close(outputPipe[1])) { throw std::runtime_error("Unable to close output pipe write side"); }

    d->outFd = inputPipe[1];
    d->inFd = outputPipe[0];
}

int ShellExecutable::GetInFd() {
    return d->inFd;
}

int ShellExecutable::GetOutFd() {
    return d->outFd;
}

ShellExecutablePrivate::~ShellExecutablePrivate() {
    if (pid > -1) {
        kill(pid, SIGKILL);
    }
    if (inFd >= 0) {
        close(inFd);
    }
    if (outFd >= 0) {
        close(outFd);
    }
}
