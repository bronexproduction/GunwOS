//
//  qemuscenario.cpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 21.07.2022.
//

#include "qemuscenario.hpp"

#include "../emulator/qemuemulator.hpp"
#include "../debugger/lldbdebugger.hpp"

QemuScenario::QemuScenario(const std::string binPath)
    : EmulatorScenario(new QemuEmulator(binPath), new LldbDebugger, binPath) {}
