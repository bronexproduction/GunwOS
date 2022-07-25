//
//  qemuscenario.hpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 21.07.2022.
//

#ifndef QEMUSCENARIO_HPP
#define QEMUSCENARIO_HPP

#include <memory>
#include <string>

#include "scenario.hpp"
#include "regset.hpp"

class QemuScenarioPrivate;
class QemuScenario: public Scenario {
    
    public:
        QemuScenario(std::string binPath);
        ~QemuScenario();

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
        QemuScenarioPrivate *d;
};

#endif // QEMUSCENARIO_HPP