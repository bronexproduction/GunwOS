//
//  log.h
//  GunwOS
//
//  Created by Artur Danielewski on 20.12.2023.
//

#ifndef LOG_H
#define LOG_H

#define LOG(MSG) k_log_log(__FILE__ ": " MSG)

void k_log_log(const char * const msg);

#endif // LOG_H
