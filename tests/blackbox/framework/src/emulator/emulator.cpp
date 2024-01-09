//
//  emulator.cpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 29.07.2022.
//

#include "emulator.hpp"

class EmulatorPrivate {

    private:
        EmulatorPrivate(const std::string binaryPath)
          : binPath(binaryPath) {}

    private:
        const std::string binPath;

    friend class Emulator;
};

Emulator::Emulator(const std::string binPath)
  : d(new EmulatorPrivate(binPath)) {}
Emulator::~Emulator() {
    delete d;
}

const std::string Emulator::GetBinaryPath() const {
    return d->binPath;
}
