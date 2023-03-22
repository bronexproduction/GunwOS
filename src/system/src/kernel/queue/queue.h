//
//  queue.h
//  GunwOS
//
//  Created by Artur Danielewski on 30.01.2023.
//

#ifndef QUEUE_H
#define QUEUE_H

/*
    Functions dedicated for additional time-consuming operations
    interrupt handlers may require to perform to process data

    To keep interrupt service routines simple
    ISR can queue its time-consuming task
    to be executed after ISR returns
*/
void k_que_dispatch(void (* const func)());
void k_que_dispatch_32_32(void (* const func)(const uint_32, const uint_32), const uint_32 p0, const uint_32 p1);

#endif // QUEUE_H
