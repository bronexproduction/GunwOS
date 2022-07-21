//
//  flatqemuscenario.cpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 21.07.2022.
//

#include "flatqemuscenario.hpp"

class FlatQEMUScenarioPrivate {
    
    private:
        int a;

    friend class FlatQEMUScenario;
};

FlatQEMUScenario::FlatQEMUScenario(): Scenario(), d(new FlatQEMUScenarioPrivate) {};
FlatQEMUScenario::~FlatQEMUScenario() {
    delete d;
} 

void FlatQEMUScenario::Prepare(void) {

}

void FlatQEMUScenario::Cleanup(void) {
    
}

void FlatQEMUScenario::SetIPAtSymbol(const std::string symbol) {
    
}
        
void FlatQEMUScenario::SetIP(uint16_t ip) {
    
}
        
void FlatQEMUScenario::SetAX(uint16_t ax) {
    
}

void FlatQEMUScenario::SetBX(uint16_t bx) {
    
}

void FlatQEMUScenario::SetCX(uint16_t cx) {
    
}

void FlatQEMUScenario::SetDX(uint16_t dx) {
    
}

void FlatQEMUScenario::ContinueUntilSymbol(const std::string symbol, size_t timeoutSeconds) {
    
}

void FlatQEMUScenario::ContinueUntil(uint16_t ip) {
    
}

void FlatQEMUScenario::CheckAX(uint16_t ax) {
    
}

void FlatQEMUScenario::CheckBX(uint16_t bx) {
    
}

void FlatQEMUScenario::CheckCX(uint16_t cx) {
    
}

void FlatQEMUScenario::CheckDX(uint16_t dx) {
    
}
