//
//  runner.hpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 21.07.2022.
//

#ifndef RUNNER_HPP
#define RUNNER_HPP

#include <memory>
#include <vector>

class Scenario;
class Runner {
    
    public:
        static Runner& Shared();

        template <class S> void Register(void) {
            scenarioRegistry.push_back(std::move(std::make_unique<S>()));
        }
        
        void Run(void);

    private:
        std::vector< std::unique_ptr<Scenario> > scenarioRegistry = std::vector< std::unique_ptr<Scenario> >();
};

#endif // RUNNER_HPP