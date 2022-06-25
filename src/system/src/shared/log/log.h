//
//  log.h
//  GunwOS
//
//  Created by Artur Danielewski on 08.03.2020.
//

enum k_log_lvl {
    FATAL   = 1,
    ERROR   = 2,
    WARNING = 3,
    INFO    = 4,
    DEBUG   = 5
};

void k_log(const enum k_log_lvl level, const char *msg);

#define LOG(LEVEL, MSG)     {k_log(LEVEL, MSG);};

#define LOG_DEBUG(MSG)      LOG(DEBUG, MSG)
#define LOG_INFO(MSG)       LOG(INFO, MSG)
#define LOG_WARN(MSG)       LOG(WARNING, MSG)
#define LOG_ERR(MSG)        LOG(ERROR, MSG)
#define LOG_FATAL(MSG)      LOG(FATAL, MSG)
