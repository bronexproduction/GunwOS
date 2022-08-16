//
//  emulatorscenario.cpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 29.07.2022.
//

#include "emulatorscenario.hpp"

#include <memory>
#include <unistd.h>
#include <sstream>
#include <signal.h>

#include "../exec.hpp"
#include "../fd.hpp"

class EmulatorScenarioPrivate {
    
    private:
        EmulatorScenarioPrivate(Emulator *e, Debugger *d, const std::string binaryPath)
          : emulator(std::unique_ptr<Emulator>(e)), 
            debugger(std::unique_ptr<Debugger>(d)), 
            binPath(binaryPath) {}

        void Cleanup(void);

        void ConfigureEmulator(void);
        void ConfigureDebugger(void);
        void AttachDebugger(void);

    private:
        std::unique_ptr<Emulator> emulator;
        std::unique_ptr<Debugger> debugger;
        const std::string binPath;

    friend class EmulatorScenario;
};

EmulatorScenario::EmulatorScenario(Emulator *e, Debugger *dbg, const std::string binPath)
    : Scenario(), d(new EmulatorScenarioPrivate(e, dbg, binPath)) {};
EmulatorScenario::~EmulatorScenario() {
    delete d;
}

void EmulatorScenario::Prepare(void) {
    d->ConfigureEmulator();
    d->ConfigureDebugger();
}

void EmulatorScenario::Cleanup(void) {
    d->Cleanup();
}

i386_regSet EmulatorScenario::ReadRegisters(void) {

}

void EmulatorScenario::SetIPAtSymbol(const std::string symbol) {
    
}
        
void EmulatorScenario::SetIP(uint16_t ip) {
    
}
        
void EmulatorScenario::SetAX(uint16_t ax) {
    
}

void EmulatorScenario::SetBX(uint16_t bx) {
    
}

void EmulatorScenario::SetCX(uint16_t cx) {
    
}

void EmulatorScenario::SetDX(uint16_t dx) {
    
}

void EmulatorScenario::ContinueUntilSymbol(const std::string symbol, size_t timeoutSeconds) {
    
}

void EmulatorScenario::ContinueUntil(uint16_t ip) {
    
}

void EmulatorScenario::CheckRegisters(const i386_regSet &ref) {

}

void EmulatorScenarioPrivate::Cleanup(void) {
    debugger.reset();
    emulator.reset();
}

void EmulatorScenarioPrivate::ConfigureEmulator(void) {
    emulator->Launch();
}

void EmulatorScenarioPrivate::ConfigureDebugger(void) {
    debugger->Launch();
    setNonBlocking(debugger->GetInFd());
    waitfd(debugger->GetInFd(), 5000);
    AttachDebugger();
}

void EmulatorScenarioPrivate::AttachDebugger() {
    waitfdstr(debugger->GetInFd(), 10000, debugger->Strings().prompt());
    flushfd(debugger->GetInFd());

    writefd(debugger->GetOutFd(), debugger->Strings().targetRemoteCmd());
    waitfd(debugger->GetInFd(), 30000);

    // Check if connection succeeded
}
