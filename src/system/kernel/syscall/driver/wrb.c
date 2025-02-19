//
//  wrb.c
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

void k_scr_drv_wrb(const procId_t procId, const uint_16 port, const uint_8 value) {

    struct gnwDeviceUHADesc desc;
    const enum gnwDeviceError error = k_dev_getByOperator(procId, &desc);
    if (error != GDE_NONE) {
        OOPS("Unexpected device operator identifier",);
        k_proc_stop(procId);
        return;
    }

    #warning TODO - checks, move to "dev"

    {
        char procIdBuffer[8] = { 0 };
        char portBuffer[8] = { 0 };
        char valueBuffer[8] = { 0 };
        int2str(procId, procIdBuffer);
        uint2hex(port, portBuffer);
        uint2hex(value, valueBuffer);
        LOG6("Proc ", procIdBuffer, " bus (", portBuffer, ") <- ", valueBuffer);
    }

    k_bus_outb(port, value);
}
