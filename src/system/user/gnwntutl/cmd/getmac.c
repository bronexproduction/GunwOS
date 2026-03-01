//
//  getmac.c
//  GunwOS
//
//  Created by Artur Danielewski on 01.03.2026.
//

#include <gunwnetwork.h>
#include <gunwoutput.h>

void cmd_getmac() {
    gnwNetworkMacAddr mac;
    
    const enum gnwNetworkError error = getMac(&mac);

    if (error != GNE_NONE) {
        printf("Error getting MAC address, code: {i}", error);
    } else {
        printf("{h}:{h}:{h}:{h}:{h}:{h}", mac.bytes[0], mac.bytes[1], mac.bytes[2], mac.bytes[3], mac.bytes[4], mac.bytes[5]);
    }
}
