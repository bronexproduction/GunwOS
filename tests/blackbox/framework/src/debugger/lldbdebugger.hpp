//
//  lldbdebugger.hpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 29.07.2022.
//

#ifndef LLDBDEBUGGER_HPP
#define LLDBDEBUGGER_HPP

#include "debugger.hpp"

class LldbDebuggerStringProvider: public DebuggerStringProvider {

    public:
        const std::string prompt() const;
        const std::string targetRemoteCmd() const;
};

class LldbDebuggerPrivate;
class LldbDebugger: public Debugger {

    public:
        LldbDebugger();

    private:
        const std::string BuildCommand() const;
};

#endif // LLDBDEBUGGER_HPP
