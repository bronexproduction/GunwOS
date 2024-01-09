//
//  runner.cpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 19.07.2022.
//

#include <algorithm>

#include "runner.hpp"
#include "scenario/scenario.hpp"

Runner& Runner::Shared() {
    // TODO: thread safety
    static std::unique_ptr<Runner> sharedInstance;

    if (!sharedInstance) {
        sharedInstance = std::make_unique<Runner>();    
    }

    return *(sharedInstance);
}

void Runner::Run(void) {

    const auto runScenario = [](const std::unique_ptr<Scenario> &scenario){
        scenario->Prepare();
        scenario->Run();
        scenario->Cleanup();
    };

    std::for_each(scenarioRegistry.begin(), scenarioRegistry.end(), runScenario);
}
