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
#include "fd.hpp"
#include "lldbstrings.hpp"

class QemuScenarioPrivate {
    
    private:
        QemuScenarioPrivate(const std::string binaryPath): binPath(binaryPath) {}
        ~QemuScenarioPrivate();

        void Cleanup(void);

        const std::string BuildQemuCommand();
        const std::string BuildDebuggerCommand();
        void ConfigureQemu(void);
        void ConfigureDebugger(void);
        void LaunchAndConfigure(const std::string command, pid_t * const pid, int * const inFd, int * const outFd);
        void AttachDebugger(void);

    private:
        const std::string binPath;
        pid_t qemuPid = -1;
        pid_t dbgPid = -1;
        int qemuIn = -1;
        int qemuOut = -1;
        int dbgIn = -1;
        int dbgOut = -1;

        const LldbStrings dbgStrings;

    friend class QemuScenario;
};

QemuScenario::QemuScenario(const std::string binPath): Scenario(), d(new QemuScenarioPrivate(binPath)) {};
QemuScenario::~QemuScenario() {
    delete d;
}

void QemuScenario::Prepare(void) {
    d->ConfigureQemu();
    d->ConfigureDebugger();
}

void QemuScenario::Cleanup(void) {
    d->Cleanup();
}

i386_regSet QemuScenario::ReadRegisters(void) {

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

void QemuScenario::CheckRegisters(const i386_regSet &ref) {

}

QemuScenarioPrivate::~QemuScenarioPrivate() {
    Cleanup();
}

void QemuScenarioPrivate::Cleanup(void) {
    if (qemuPid > -1) {
        kill(qemuPid, SIGKILL);
    }
    if (dbgPid > -1) {
        kill(dbgPid, SIGKILL);
    }
    
    if (qemuIn >= 0) {
        close(qemuIn);
    }
    if (qemuOut >= 0) {
        close(qemuOut);
    }
    if (dbgIn >= 0) {
        close(dbgIn);
    }
    if (dbgOut >= 0) {
        close(dbgOut);
    }

    qemuPid = -1;
    dbgPid = -1;
    qemuIn = -1;
    qemuOut = -1;
    dbgIn = -1;
    dbgOut = -1;
}

const std::string QemuScenarioPrivate::BuildQemuCommand() {
    std::stringstream ss;
    ss << "bash $SCRIPTS_DIR/qemu_run_debug.sh " << binPath;
    return ss.str();
}

const std::string QemuScenarioPrivate::BuildDebuggerCommand() {
    std::stringstream ss;
    ss << "lldb 2>&1";
    return ss.str();
}

void QemuScenarioPrivate::ConfigureQemu(void) {
    LaunchAndConfigure(BuildQemuCommand(), &qemuPid, &qemuIn, &qemuOut);
}

void QemuScenarioPrivate::ConfigureDebugger(void) {
    LaunchAndConfigure(BuildDebuggerCommand(), &dbgPid, &dbgIn, &dbgOut);
    setNonBlocking(dbgIn);
    waitfd(dbgIn, 5000);
    AttachDebugger();
}

void QemuScenarioPrivate::LaunchAndConfigure(const std::string command, pid_t * const pid, int * const inFd, int * const outFd) {
    if (!(pid && inFd && outFd)) {
        throw std::runtime_error("NULL pointer passed as argument");
    }

    int inputPipe[2];
    int outputPipe[2];

    if (pipe(inputPipe)) {
        throw std::runtime_error("Failed to create input pipe");
    }
    if (pipe(outputPipe)) {
        throw std::runtime_error("Failed to create output pipe");
    }
    
    *pid = spawnShell(command, [=](){
        if (close(STDIN_FILENO)) {                      throw std::runtime_error("Unable to close stdin"); }
        if (dup2(inputPipe[0], STDIN_FILENO) < 0) {     throw std::runtime_error("Unable to duplicate input pipe descriptor"); }
        if (close(inputPipe[0])) {                      throw std::runtime_error("Unable to close input pipe read side"); }
        if (close(inputPipe[1])) {                      throw std::runtime_error("Unable to close input pipe write side"); }
        if (close(STDOUT_FILENO)) {                     throw std::runtime_error("Unable to close stdout"); }
        if (dup2(outputPipe[1], STDOUT_FILENO) < 0) {   throw std::runtime_error("Unable to duplicate output pipe descriptor"); }
        if (close(outputPipe[0])) {                     throw std::runtime_error("Unable to close output pipe read side"); }
        if (close(outputPipe[1])) {                     throw std::runtime_error("Unable to close output pipe write side"); }
    });
    
    if (*pid < 0) {
        std::stringstream ss;
        ss << "Unable to launch: " << binPath;
        throw std::runtime_error(ss.str());
    }

    if (close(inputPipe[0])) {  throw std::runtime_error("Unable to close input pipe read side"); }
    if (close(outputPipe[1])) { throw std::runtime_error("Unable to close output pipe write side"); }

    *outFd = inputPipe[1];
    *inFd = outputPipe[0];
}

void QemuScenarioPrivate::AttachDebugger() {
    waitfdstr(dbgIn, 10000, dbgStrings.prompt);
    flushfd(dbgIn);

    writefd(dbgOut, dbgStrings.targetRemoteString);
    waitfd(dbgIn, 30000);

    // Check if connection succeeded

    printf("ridink\n");
    for (int i=0; i<10000; ++i) {
        char b;
        read(dbgIn, &b, 1);
        printf("%c", b);
    }
    printf("Juz nie riding\n");
}
