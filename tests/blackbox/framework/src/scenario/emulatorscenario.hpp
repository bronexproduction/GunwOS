//
//  emulatorscenario.hpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 29.07.2022.
//

#ifndef EMULATORSCENARIO_HPP
#define EMULATORSCENARIO_HPP

#include <string>

#include "scenario.hpp"
#include "../emulator/emulator.hpp"
#include "../debugger/debugger.hpp"
#include "../regset.hpp"

class EmulatorScenarioPrivate;
class EmulatorScenario: public Scenario {

    public:
        EmulatorScenario(Emulator*, Debugger*, const std::string binPath);
        ~EmulatorScenario();

    public:
        void Prepare(void);
        void Cleanup(void);

    public:
        i386_regSet ReadRegisters();

        void SetIPAtSymbol(const std::string symbol);
        void SetIP(uint16_t ip);

        void SetAX(uint16_t ax);
        void SetBX(uint16_t bx);
        void SetCX(uint16_t cx);
        void SetDX(uint16_t dx);

        void ContinueUntilSymbol(const std::string symbol, size_t timeoutSeconds);
        void ContinueUntil(uint16_t ip);

        void CheckRegisters(const i386_regSet &ref);

    private:
        EmulatorScenarioPrivate * const d;
};

#endif // EMULATORSCENARIO_HPP
