//
//  runner.cpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 19.07.2022.
//

#include "runner.hpp"

#include <vector>
#include <algorithm>

#include "scenario.hpp"

Runner Runner::shared = Runner();
Runner::Runner(): d(std::make_unique<RunnerPrivate>()) {}
Runner::~Runner() {}

class RunnerPrivate {
    
    private:
        std::vector< std::unique_ptr<Scenario> > scenarioRegistry;

    friend class Runner;
};

template <class S> void Runner::Register() {
    
    d.get()->scenarioRegistry.push_back(std::move(std::make_unique<S>()));
}

void Runner::Run(void) {

    const auto runScenario = [](const std::unique_ptr<Scenario> &scenario){ 
        scenario.get()->run();
    };

    std::for_each(d.get()->scenarioRegistry.begin(), d.get()->scenarioRegistry.end(), runScenario);
}
