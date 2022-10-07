//
//  qemuscenario.hpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 21.07.2022.
//

#ifndef QEMUSCENARIO_HPP
#define QEMUSCENARIO_HPP

#include "emulatorscenario.hpp"
#include "../emulator/qemuemulator.hpp"
#include "../debugger/lldbdebugger.hpp"

class QemuScenario: public EmulatorScenario {

    public:
        QemuScenario(const std::string binPath);
};

#endif // QEMUSCENARIO_HPP
