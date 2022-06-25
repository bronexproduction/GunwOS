//
//  log.c
//  GunwOS
//
//  Created by Artur Danielewski on 08.03.2020.
//

#include "log.h"

#include "../../service/terminal/terminal.h"

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

    s_trm_puts(lvlString);
    s_trm_puts(": ");
    s_trm_puts(msg);
    s_trm_putc('\n');

    while(level == FATAL);
}
