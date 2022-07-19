//
//  run.cpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 18.07.2022.
//

#include <vector>
#include <memory>
#include <algorithm>

#include <gunwtest>

std::vector< std::unique_ptr<Scenario> > g_scenarioRegistry;

int main() {

    const auto runScenario = [](const std::unique_ptr<Scenario> &scenario){ 
        scenario.get()->run();
    };

    std::for_each(g_scenarioRegistry.begin(), g_scenarioRegistry.end(), runScenario);

    return 0;
}