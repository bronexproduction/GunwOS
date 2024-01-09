//
//  debugger.cpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 29.07.2022.
//

#include "debugger.hpp"
#include <memory>

class DebuggerPrivate {

    private:
        DebuggerPrivate(const DebuggerStringProvider * const p)
          : strings(std::unique_ptr<const DebuggerStringProvider>(p)) {}

    private:
        const std::unique_ptr<const DebuggerStringProvider> strings;

    friend class Debugger;
};

Debugger::Debugger(const DebuggerStringProvider * const p): d(new DebuggerPrivate(p)) {}
Debugger::~Debugger() {
    delete d;
}

const DebuggerStringProvider& Debugger::Strings() const {
    return *(d->strings);
}
