//
//  data.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.12.2023.
//

#include "data.h"

struct ipcListener ipcListenerRegister[MAX_IPC_LISTENER];
struct ipcReply ipcReplyRegister[MAX_IPC_TOKEN];
