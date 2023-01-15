//
//  log.c
//  GunwOS
//
//  Created by Artur Danielewski on 08.03.2020.
//

#include "log.h"

#include "../driver/terminal/terminal.h"

void k_log(const enum k_log_lvl level, const char *msg) {
    char *lvlString;
    
    switch (level) {
        case DEBUG:
            lvlString = "DEBUG";
            break;
        case INFO:
            lvlString = "INFO";
            break;
        case WARNING:
            lvlString = "WARNING";
            break;
        case ERROR:
            lvlString = "ERROR";
            break;
        case FATAL:
            lvlString = "FATAL";
            break;
        default:
            return;
    }

    c_trm_puts(lvlString);
    c_trm_puts(": ");
    c_trm_puts(msg);
    c_trm_putc('\n');

    while(level == FATAL);
}