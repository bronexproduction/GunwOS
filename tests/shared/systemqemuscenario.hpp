//
//  systemqemuscenario.hpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 21.07.2022.
//

#ifndef SYSTEMQEMUSCENARIO_HPP
#define SYSTEMQEMUSCENARIO_HPP

#include <gunwtest>

class SystemQemuScenarioPrivate;
class SystemQemuScenario: public QemuScenario {
    
    public:
        SystemQemuScenario();
        ~SystemQemuScenario();

    private:
        SystemQemuScenarioPrivate *d;
};

#endif // SYSTEMQEMUSCENARIO_HPP