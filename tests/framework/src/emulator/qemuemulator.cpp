//
//  qemuemulator.cpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 29.07.2022.
//

#include "qemuemulator.hpp"

#include <sstream>

QemuEmulator::QemuEmulator(const std::string binPath)
  : Emulator(binPath) {}

const std::string QemuEmulator::BuildCommand() const {
    std::stringstream ss;
    ss << "bash $SCRIPTS_DIR/qemu_run_debug.sh " << GetBinaryPath();
    return ss.str();
}
