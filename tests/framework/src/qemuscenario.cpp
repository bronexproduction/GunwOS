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

#include "spawn.hpp"

class QEMUScenarioPrivate {
    
    private:
        QEMUScenarioPrivate(const std::string binaryPath): binPath(binaryPath) {}

        const std::string BuildQemuCommand();

    private:
        const std::string binPath;
        pid_t qemuPid = -1;

    friend class QEMUScenario;
};

QEMUScenario::QEMUScenario(const std::string binPath): Scenario(), d(new QEMUScenarioPrivate(binPath)) {};
QEMUScenario::~QEMUScenario() {
    delete d;
} 

const std::string QEMUScenarioPrivate::BuildQemuCommand() {
    std::stringstream ss;
    ss << "bash $SCRIPTS_DIR/qemu_run_debug.sh " << binPath;
    return ss.str();
}

void QEMUScenario::Prepare(void) {
    d->qemuPid = spawnShell(d->BuildQemuCommand());
    if (d->qemuPid < 0) {
        throw std::runtime_error("Unable to launch QEMU");
    }
    // launch & attach GDB
}

void QEMUScenario::Cleanup(void) {
     kill(d->qemuPid, SIGKILL);
}

void QEMUScenario::SetIPAtSymbol(const std::string symbol) {
    
}
        
void QEMUScenario::SetIP(uint16_t ip) {
    
}
        
void QEMUScenario::SetAX(uint16_t ax) {
    
}

void QEMUScenario::SetBX(uint16_t bx) {
    
}

void QEMUScenario::SetCX(uint16_t cx) {
    
}

void QEMUScenario::SetDX(uint16_t dx) {
    
}

void QEMUScenario::ContinueUntilSymbol(const std::string symbol, size_t timeoutSeconds) {
    
}

void QEMUScenario::ContinueUntil(uint16_t ip) {
    
}

void QEMUScenario::CheckAX(uint16_t ax) {
    
}

void QEMUScenario::CheckBX(uint16_t bx) {
    
}

void QEMUScenario::CheckCX(uint16_t cx) {
    
}

void QEMUScenario::CheckDX(uint16_t dx) {
    
}
