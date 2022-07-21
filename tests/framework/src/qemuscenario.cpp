//
//  qemuscenario.cpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 21.07.2022.
//

#include "qemuscenario.hpp"

class QEMUScenarioPrivate {
    
    private:
        QEMUScenarioPrivate(const std::string binaryPath): binPath(binaryPath) {}

    private:
        const std::string binPath;

    friend class QEMUScenario;
};

QEMUScenario::QEMUScenario(const std::string binPath): Scenario(), d(new QEMUScenarioPrivate(binPath)) {};
QEMUScenario::~QEMUScenario() {
    delete d;
} 

void QEMUScenario::Prepare(void) {

}

void QEMUScenario::Cleanup(void) {
    
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
