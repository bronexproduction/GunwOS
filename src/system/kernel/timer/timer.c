//
//  timer.c
//  GunwOS
//
//  Created by Artur Danielewski on 19.03.2020.
//

#include <types.h>

#include <error/panic.h>
#include <error/fug.h>

static time_t ticks = 0;
static time_t ms = 0;

static uint_32 pitDrift = 0;
static uint_32 driftCurrent = 0;

#define MAX_MS_HANDLERS     1
static size_t msHandlerCount = 0;

static void (*auxMsHandlers[MAX_MS_HANDLERS]) (void);

static void k_tmr_tick() {
    extern const uint_32 k_pit_freq;

    ++ticks;

    /*
        Calculating timer drift value

        Drift is always positive (look at k_tmr_init() how it is calculated)
        
        When drift value reaches PIT operating frequency in Hz we should ignore this millisecond increment
        (drift value accumulates to single tick unit)
    */
    driftCurrent += pitDrift;           // Add tick drift value to current counter
    if (driftCurrent >= k_pit_freq) {   // If current drift over operating frequency
        driftCurrent %= k_pit_freq;     // Calculate drift amount left
        return;                         // Ignore millisecond
    }

    ++ms;
    
    for (size_t i = 0; i < msHandlerCount; ++i) {
        auxMsHandlers[i]();
    }
}

void k_tmr_init() {
    extern const uint_32 k_pit_freq;
    extern uint_16 k_pit_evFreq;
    extern void (*k_pit_routine)();
    
    k_pit_routine = k_tmr_tick;
    k_pit_evFreq = 1000;
    pitDrift = k_pit_freq % k_pit_evFreq;
}

time_t k_tmr_getTicks() {
    return ticks;
}

time_t k_tmr_getSec() {
    return ms / 1000;
}

time_t k_tmr_getMs() {
    return ms;
}

void k_tmr_sleepms(const unsigned int milliseconds) {
    const time_t targetTime = ms + milliseconds;

    while(ms < targetTime);
}

void k_tmr_sleeps(const unsigned int seconds) {
    k_tmr_sleepms(seconds * 1000);
}

void k_tmr_regMsHandler(void (*handler)()) {
    CHECKPTR(handler);
    if (msHandlerCount >= MAX_MS_HANDLERS) {
        OOPS("Max auxiliary tick handler count reached");
    }

    auxMsHandlers[msHandlerCount++] = handler;
}
