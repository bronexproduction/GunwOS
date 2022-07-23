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

        const std::string BuildQemuCommand();
        const std::string BuildGdbCommand();
        void AttachGdb();

    private:
        const std::string binPath;
        pid_t qemuPid = -1;
        pid_t gdbPid = -1;

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
    d->AttachGdb();
    
    while(1);
}

void QemuScenario::Cleanup(void) {
    kill(d->qemuPid, SIGKILL);
    kill(d->gdbPid, SIGKILL);
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

void QemuScenarioPrivate::AttachGdb() {
    // Call pipe() twice, one for pipe-to-child, one for pipe-from-child, yielding 4 file descriptors.
    gdbPid = execShell(BuildGdbCommand());
    {
        // In child:
        // Call close() on standard input (file descriptor 0).
        // Call dup() - or dup2() - to make read end of pipe-to-child into standard input.
        // Call close() on read end of pipe-to-child.
        // Call close() on write end of pipe-to-child.
        // Call close() on standard output (file descriptor 1).
        // Call dup() - or dup2() - to make write end of pipe-from-child into standard output
        // Call close() on write end of pipe-from-child.
        // Call close() on read end of pipe-from-child.
        // Execute the required program.
    }
    {
        // In parent:
        // Call close on read end of pipe-to-child.
        // Call close on write end of pipe-from-child.
        // Loop sending data to child on write end of pipe-to child and reading data from child on read end of pipe-from-child
        // When no more to send to child, close write end of pipe-to-child.
        // When all data received, close read end of pipe-from-child.
        // Note how many closes there are, especially in the child. If you use dup2(), you don't have to close standard input and output explicitly; however, dup() works correctly if you do the explicit closes. Also note that neither dup() nor dup2() closes the file descriptor that is duplicated. If you omit closing the pipes, then neither program can detect EOF correctly; the fact that the current process can still write to a pipe means that there is no EOF on the pipe, and the program will hang indefinitely.

        // Note that this solution does not alter standard error for the child; it goes to the same place as standard error for the parent. Often, this is correct. If you have a specific requirement that error messages from the child are handled differently, then take appropriate action on the child's standard error too.
    }
    if (gdbPid < 0) {
        throw std::runtime_error("Unable to launch GDB");
    }
}
