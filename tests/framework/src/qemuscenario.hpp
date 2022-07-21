//
//  qemuscenario.hpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 21.07.2022.
//

#ifndef QEMUSCENARIO_HPP
#define QEMUSCENARIO_HPP

#include <memory>

#include "scenario.hpp"

class QEMUScenarioPrivate;
class QEMUScenario: public Scenario {
    
    public:
        QEMUScenario(std::string binPath);
        ~QEMUScenario();

    public:
        void Prepare(void);
        void Cleanup(void);

    public:
        void SetIPAtSymbol(const std::string symbol);
        void SetIP(uint16_t ip);

        void SetAX(uint16_t ax);
        void SetBX(uint16_t bx);
        void SetCX(uint16_t cx);
        void SetDX(uint16_t dx);

        void ContinueUntilSymbol(const std::string symbol, size_t timeoutSeconds);
        void ContinueUntil(uint16_t ip);

        void CheckAX(uint16_t ax);
        void CheckBX(uint16_t bx);
        void CheckCX(uint16_t cx);
        void CheckDX(uint16_t dx);

    private:
        QEMUScenarioPrivate *d;
};

#endif // QEMUSCENARIO_HPP