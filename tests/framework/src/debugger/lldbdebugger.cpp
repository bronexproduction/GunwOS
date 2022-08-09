//
//  lldbdebugger.cpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 29.07.2022.
//

#include "lldbdebugger.hpp"

const std::string LldbDebuggerStringProvider::prompt() const {
    return "(lldb)"; 
}

const std::string LldbDebuggerStringProvider::targetRemoteCmd() const {
    return "target remote :1234"; 
}

LldbDebugger::LldbDebugger(): Debugger(new LldbDebuggerStringProvider) {}

const std::string LldbDebugger::BuildCommand() const {
    return "lldb -X 2>&1";
}
