//
//  systemqemuscenario.hpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 21.07.2022.
//

#ifndef SYSTEMQEMUSCENARIO_HPP
#define SYSTEMQEMUSCENARIO_HPP

#include <gunwtest>

class SystemQEMUScenarioPrivate;
class SystemQEMUScenario: public QEMUScenario {
    
    public:
        SystemQEMUScenario();
        ~SystemQEMUScenario();

    private:
        SystemQEMUScenarioPrivate *d;
};

#endif // SYSTEMQEMUSCENARIO_HPP