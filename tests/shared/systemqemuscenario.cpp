//
//  systemqemuscenario.cpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 21.07.2022.
//

#include "systemqemuscenario.hpp"

class SystemQemuScenarioPrivate {
    
    friend class SystemQemuScenario;
};

SystemQemuScenario::SystemQemuScenario(): QemuScenario("$BUILD_DIR/gunwos.img"), d(new SystemQemuScenarioPrivate) {};
SystemQemuScenario::~SystemQemuScenario() {
    delete d;
} 
