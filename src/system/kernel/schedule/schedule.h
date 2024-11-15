//
//  schedule.h
//  GunwOS
//
//  Created by Artur Danielewski on 30.01.2023.
//

#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <hal/proc/proc.h>

/*
    Called by the ISR
    in case code got dispatched by an interrupt handler
    to be executed on ISR return
*/
void __attribute__((cdecl)) k_proc_schedule_intNeedsKernelHandling(const uint_32 refEsp);

/*
    Called by the kernel queue
    in case all the dispatched code got executed
    and execution can be switched to another process
*/
void k_proc_schedule_onKernelHandlingFinished();

/*
    Called by the k_proc_hatch
    in case a new process got hatched
*/
void k_proc_schedule_didHatch(const procId_t procId);

/*
    Notifies scheduler about active process state change
    outside the scheduler
    
    New process state may need schedule recalculation
*/
void k_proc_schedule_processStateDidChange();

#endif // SCHEDULE_H
