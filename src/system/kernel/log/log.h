//
//  log.h
//  GunwOS
//
//  Created by Artur Danielewski on 20.12.2023.
//

#ifndef LOG_H
#define LOG_H

#include <types.h>

#define LOG_START LOG_NBR(__FILE__ ": ")
#define LOG_NBR(MSG) k_log_log(MSG);
#define LOG_END k_log_log("\n");
#define LOG_BLOCK(LOGS) { LOG_START; { LOGS; } LOG_END; }

#define LOG(MSG) LOG_BLOCK(LOG_NBR(MSG);)
#define LOG2(MSG1, MSG2) LOG_BLOCK(LOG_NBR(MSG1); LOG_NBR(MSG2);)

void k_log_log(const char * const absMsgPtr);
void k_log_logd(const data_t absMsgData);

#endif // LOG_H
