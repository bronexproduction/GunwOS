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

#include <string.h>

uint_8 k_scr_drv_rdb(const procId_t procId, const size_t deviceId, const uint_16 port) {

    if (k_dev_operatorOf(deviceId) != procId) {
        k_proc_stop(procId);
        OOPS("Invalid device operator identifier", 0);
    }

    #warning TODO - checks, move to "dev"

    return k_bus_inb(port);
}
