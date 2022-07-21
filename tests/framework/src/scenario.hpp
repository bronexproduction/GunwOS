//
//  scenario.hpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 21.07.2022.
//

#ifndef SCENARIO_HPP
#define SCENARIO_HPP

class Scenario {

    public:
        virtual void run(void) = 0;
        virtual ~Scenario() {}
};

#define SCENARIO(CLASS, NAME, RUN) class Scenario_ ## NAME : public CLASS { \
    public:                                                                 \
        void run(void) {RUN;}                                               \
};                                                                          \
static void NAME ## _registration(void) __attribute__((constructor));       \
static void NAME ## _registration(void) {                                   \
    Runner::Shared().Register< Scenario_ ## NAME >();                       \
}


#endif // SCENARIO_HPP