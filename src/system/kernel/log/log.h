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

#define LOG(MSG) { LOG_START; LOG_NBR(MSG); LOG_END; }

void k_log_log(const char * const absMsgPtr);
void k_log_logd(const data_t absMsgData);

#endif // LOG_H
