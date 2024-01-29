//
//  emulator.hpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 29.07.2022.
//

#ifndef EMULATOR_HPP
#define EMULATOR_HPP

#include "../shellexecutable.hpp"

class EmulatorPrivate;
class Emulator: public ShellExecutable {

    public:
        Emulator(const std::string binPath);
        ~Emulator();

    protected:
        const std::string GetBinaryPath() const;

    private:
        EmulatorPrivate * const d;
};

#endif // EMULATOR_HPP
