//
//  log.c
//  GunwOS
//
//  Created by Artur Danielewski on 08.03.2020.
//

#include "log.h"

#include <stdgunw/string.h>

#include <driver/terminal/terminal.h>

#warning LOG functions should not use terminal as it works at the moment
#warning create separate kernel output independent of "terminal"

void k_log(const enum k_log_lvl level, const char *msg) {
    k_logl(level, msg, strlen(msg));
}

void k_logl(const enum k_log_lvl level, const char *msg, const size_t len) {
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

    k_trm_puts(lvlString);
    k_trm_puts(": ");
    k_trm_putsl(msg, len);
    k_trm_putc('\n');

    while(level == FATAL);
}
