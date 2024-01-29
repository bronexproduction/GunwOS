//
//  debugger.hpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 29.07.2022.
//

#ifndef DEBUGGER_HPP
#define DEBUGGER_HPP

#include "../shellexecutable.hpp"

class DebuggerStringProvider {

    public:
        virtual const std::string prompt() const = 0;
        virtual const std::string targetRemoteCmd() const = 0;
};

class DebuggerPrivate;
class Debugger: public ShellExecutable {

    public:
        Debugger(const DebuggerStringProvider * const p);
        ~Debugger();

    public:
        const DebuggerStringProvider& Strings() const;

    private:
        DebuggerPrivate * const d;
};

#endif // DEBUGGER_HPP
