//
//  queue.h
//  GunwOS
//
//  Created by Artur Danielewski on 30.01.2023.
//

#ifndef QUEUE_H
#define QUEUE_H

/*
    Function dedicated for interrupt handlers
    which may require some time to process data

    To keep interrupt service routines simple
    ISR can queue its time-consuming task
    to be executed after ISR returns
*/
void k_que_dispatch(void (* const func)());

#endif // QUEUE_H
