//
//  run.cpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 18.07.2022.
//

#include <vector>
#include <gunwtest>

std::vector< const std::unique_ptr<Scenario> > g_scenarios;

int main() {

    const auto runScenario = [](const std::unique_ptr<Scenario> &scenario){ 
        printf("asdf\n");
    };

    std::for_each(g_scenarios.begin(), g_scenarios.end(), runScenario);

    return 0;
}