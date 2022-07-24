//
//  qemuscenario.cpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 21.07.2022.
//

#include "qemuscenario.hpp"

#include <unistd.h>
#include <sstream>
#include <signal.h>

#include "exec.hpp"

class QemuScenarioPrivate {
    
    private:
        QemuScenarioPrivate(const std::string binaryPath): binPath(binaryPath) {}

        void Cleanup(void);

        const std::string BuildQemuCommand();
        const std::string BuildGdbCommand();
        void ConfigureGdb(void);

    private:
        const std::string binPath;
        pid_t qemuPid = -1;
        pid_t gdbPid = -1;
        int gdbIn = -1;
        int gdbOut = -1;

    friend class QemuScenario;
};

QemuScenario::QemuScenario(const std::string binPath): Scenario(), d(new QemuScenarioPrivate(binPath)) {};
QemuScenario::~QemuScenario() {
    delete d;
}

void QemuScenario::Prepare(void) {
    d->qemuPid = spawnShell(d->BuildQemuCommand());
    if (d->qemuPid < 0) {
        throw std::runtime_error("Unable to launch QEMU");
    }
    
    d->ConfigureGdb();
    
    while(1);
}

void QemuScenario::Cleanup(void) {
    d->Cleanup();
}

void QemuScenario::SetIPAtSymbol(const std::string symbol) {
    
}
        
void QemuScenario::SetIP(uint16_t ip) {
    
}
        
void QemuScenario::SetAX(uint16_t ax) {
    
}

void QemuScenario::SetBX(uint16_t bx) {
    
}

void QemuScenario::SetCX(uint16_t cx) {
    
}

void QemuScenario::SetDX(uint16_t dx) {
    
}

void QemuScenario::ContinueUntilSymbol(const std::string symbol, size_t timeoutSeconds) {
    
}

void QemuScenario::ContinueUntil(uint16_t ip) {
    
}

void QemuScenario::CheckAX(uint16_t ax) {
    
}

void QemuScenario::CheckBX(uint16_t bx) {
    
}

void QemuScenario::CheckCX(uint16_t cx) {
    
}

void QemuScenario::CheckDX(uint16_t dx) {
    
}

void QemuScenarioPrivate::Cleanup(void) {
    if (qemuPid > -1) {
        kill(qemuPid, SIGKILL);
    }
    if (gdbPid > -1) {
        kill(gdbPid, SIGKILL);
    }
    
    if (gdbIn >= 0) {
        close(gdbIn);
    }
    if (gdbOut >= 0) {
        close(gdbOut);
    }

    qemuPid = -1;
    gdbPid = -1;
    gdbIn = -1;
    gdbOut = -1;
}

const std::string QemuScenarioPrivate::BuildQemuCommand() {
    std::stringstream ss;
    ss << "bash $SCRIPTS_DIR/qemu_run_debug.sh " << binPath;
    return ss.str();
}

const std::string QemuScenarioPrivate::BuildGdbCommand() {
    std::stringstream ss;
    ss << "gdb -ex 'target remote :1234' -ex 'attach'";
    return ss.str();
}

void QemuScenarioPrivate::ConfigureGdb(void) {
    int gdbInputPipe[2];
    int gdbOutputPipe[2];

    if (pipe(gdbInputPipe)) {
        throw std::runtime_error("Failed to create GDB input pipe");
    }
    if (pipe(gdbOutputPipe)) {
        throw std::runtime_error("Failed to create GDB output pipe");
    }
    
    gdbPid = spawnShell(BuildGdbCommand(), [=](){
        if (close(STDIN_FILENO)) {                          throw std::runtime_error("GDB process: Unable to close stdin"); }
        if (dup2(gdbInputPipe[0], STDIN_FILENO) < 0) {      throw std::runtime_error("GDB process: Unable to duplicate input pipe descriptor"); }
        if (close(gdbInputPipe[0])) {                       throw std::runtime_error("GDB process: Unable to close input pipe read side"); }
        if (close(gdbInputPipe[1])) {                       throw std::runtime_error("GDB process: Unable to close input pipe write side"); }
        if (close(STDOUT_FILENO)) {                         throw std::runtime_error("GDB process: Unable to close stdout"); }
        if (dup2(gdbOutputPipe[1], STDOUT_FILENO) < 0) {    throw std::runtime_error("GDB process: Unable to duplicate output pipe descriptor"); }
        if (close(gdbOutputPipe[0])) {                      throw std::runtime_error("GDB process: Unable to close output pipe read side"); }
        if (close(gdbOutputPipe[1])) {                      throw std::runtime_error("GDB process: Unable to close output pipe write side"); }
    });
    
    if (gdbPid < 0) {
        throw std::runtime_error("Unable to launch GDB");
    }

    if (close(gdbInputPipe[0])) {   throw std::runtime_error("Unable to close input pipe read side"); }
    if (close(gdbOutputPipe[1])) {  throw std::runtime_error("Unable to close output pipe write side"); }
    gdbIn = gdbInputPipe[1];
    gdbOut = gdbOutputPipe[0];
}
