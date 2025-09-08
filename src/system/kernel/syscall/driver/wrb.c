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

void k_scr_drv_wrb(const procId_t procId, const size_t deviceId, const uint_16 port, const uint_8 value) {

    if (k_dev_operatorOf(deviceId) != procId) {
        OOPS("Invalid device operator identifier",);
        k_proc_stop(procId);
        return;
    }

    #warning TODO - checks, move to "dev"

    k_bus_outb(port, value);
}
