//
//  log.h
//  GunwOS
//
//  Created by Artur Danielewski on 20.12.2023.
//

#ifndef LOG_H
#define LOG_H

#define LOG_START LOG_NBR(__FILE__ ": ")
#define LOG_NBR(MSG) k_log_log(MSG);
#define LOG_END k_log_log("\n");

#define LOG(MSG) { LOG_START; LOG_NBR(MSG); LOG_END; }
#define LOG2(MSG1, MSG2) { LOG_START; LOG_NBR(MSG1); LOG_NBR(MSG2); LOG_END; }

void k_log_log(const char * const msg);

#endif // LOG_H
