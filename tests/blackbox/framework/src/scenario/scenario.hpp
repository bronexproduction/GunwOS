//
//  scenario.hpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 21.07.2022.
//

#ifndef SCENARIO_HPP
#define SCENARIO_HPP

#include <cstdio>

class Scenario {

    public:
        virtual ~Scenario() {}

        virtual void Prepare(void) = 0;
        virtual void Run(void) = 0;
        virtual void Cleanup(void) = 0;

};

#define SCENARIO(CLASS, NAME, RUN) class Scenario_ ## NAME : public CLASS { \
    public:                                                                 \
        void Prepare(void) {                                                \
            printf(#NAME " - Preparing...\n");                              \
            CLASS::Prepare();                                               \
        }                                                                   \
        void Run(void) {                                                    \
            printf(#NAME " - Running...\n");                                \
            {                                                               \
                RUN;                                                        \
            }                                                               \
            printf(#NAME " - Run finished\n");                              \
        }                                                                   \
        void Cleanup(void) {                                                \
            printf(#NAME " - Cleaning up...\n");                            \
            CLASS::Cleanup();                                               \
            printf(#NAME " - Done\n");                                      \
        }                                                                   \
};                                                                          \
static void NAME ## _registration(void) __attribute__((constructor));       \
static void NAME ## _registration(void) {                                   \
    Runner::Shared().Register< Scenario_ ## NAME >();                       \
}


#endif // SCENARIO_HPP