//
//  runner.cpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 19.07.2022.
//

#include <gunwtest>

#include <algorithm>

Runner& Runner::Shared() {
    // TODO: thread safety
    static std::unique_ptr<Runner> sharedInstance;

    if (!sharedInstance.get()) {
        sharedInstance = std::make_unique<Runner>();    
    }

    return *(sharedInstance.get());
}

void Runner::Run(void) {

    const auto runScenario = [](const std::unique_ptr<Scenario> &scenario){
        scenario.get()->run();
    };

    std::for_each(scenarioRegistry.begin(), scenarioRegistry.end(), runScenario);
}
