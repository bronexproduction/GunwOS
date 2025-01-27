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

uint_8 k_scr_drv_rdb(const procId_t procId, const uint_16 port) {
    
    struct gnwDeviceUHADesc desc;
    const enum gnwDeviceError error = k_dev_getByOperator(procId, &desc);
    if (error != GDE_NONE) {
        OOPS("Unexpected device operator identifier", 0);
        k_proc_stop(procId);
        return 0;
    }

    #warning TODO - checks, move to "dev"

    return k_bus_inb(port);
}
