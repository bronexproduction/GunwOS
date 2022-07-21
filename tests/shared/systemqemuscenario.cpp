//
//  systemqemuscenario.cpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 21.07.2022.
//

#include "systemqemuscenario.hpp"

class SystemQEMUScenarioPrivate {
    
    friend class SystemQEMUScenario;
};

SystemQEMUScenario::SystemQEMUScenario(): QEMUScenario("gunwos.img"), d(new SystemQEMUScenarioPrivate) {};
SystemQEMUScenario::~SystemQEMUScenario() {
    delete d;
} 
