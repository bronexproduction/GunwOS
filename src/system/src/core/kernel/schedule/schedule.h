//
//  schedule.h
//  GunwOS
//
//  Created by Artur Danielewski on 30.01.2023.
//

#ifndef SCHEDULE_H
#define SCHEDULE_H

/*
    Called by the ISR
    in case code got dispatched by an interrupt handler
    to be executed on ISR return
*/
void k_proc_schedule_intNeedsKernelHandling();

/*
    Called by the kernel queue
    in case all the dispatched code got executed
    and execution can be switched to another process
*/
void k_proc_schedule_onKernelHandlingFinished();

/*
    Called by the k_proc_spawn
    in case a new process got spawned
*/
void k_proc_schedule_didSpawn();

#endif // SCHEDULE_H