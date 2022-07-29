//
//  qemuemulator.hpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 29.07.2022.
//

#ifndef QEMUEMULATOR_HPP
#define QEMUEMULATOR_HPP

#include "emulator.hpp"

class QemuEmulator: public Emulator {

    public:
        QemuEmulator(const std::string binPath);

    private:
        const std::string BuildCommand() const;
};

#endif // QEMUEMULATOR_HPP
