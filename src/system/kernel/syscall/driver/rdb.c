//
//  rdb.c
//  GunwOS
//
//  Created by Artur Danielewski on 20.01.2025.
//

#include <dev/dev.h>
#include <error/panic.h>
#include <hal/proc/proc.h>
#include <hal/io/bus.h>

#include <log/log.h>
#include <string.h>

uint_8 k_scr_drv_rdb(const procId_t procId, const uint_16 port) {
    
    struct gnwDeviceUHADesc desc;
    const enum gnwDeviceError error = k_dev_getByOperator(procId, &desc);
    if (error != GDE_NONE) {
        OOPS("Unexpected device operator identifier", 0);
        k_proc_stop(procId);
        return 0;
    }

    #warning TODO - checks, move to "dev"

    const uint_8 result = k_bus_inb(port);

    {
        char procIdBuffer[8] = { 0 };
        char portBuffer[8] = { 0 };
        char resultBuffer[8] = { 0 };
        int2str(procId, procIdBuffer);
        uint2hex(port, portBuffer);
        uint2hex(result, resultBuffer);
        LOG6("Proc ", procIdBuffer, " bus (", portBuffer, ") -> ", resultBuffer);
    }

    return result;
}
