//
//  runner.hpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 19.07.2022.
//

#ifndef RUNNER_HPP
#define RUNNER_HPP

#include <memory>

class RunnerPrivate;
class Runner {
    
    public:
        static Runner shared;

        template <typename S>
        void Register(void);
        
        void Run(void);

    private:
        Runner();
        ~Runner();

        std::unique_ptr<RunnerPrivate> d;
};

#endif // RUNNER_HPP
