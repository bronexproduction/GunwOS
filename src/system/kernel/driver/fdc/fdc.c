//
//  fdc.c
//  GunwOS
//
//  Created by Artur Danielewski on 26.03.2023.
//  
//  Docs:
//  * Intel 82077AA Datasheet
//  * https://wiki.osdev.org/Floppy_Disk_Controller
//

#include <gunwdev.h>
#include <gunwdrv.h>
#include "common/data.h"
#include "common/io.h"
#include "cmd/cmd.h"
#include "proc/proc.h"
#include <error/panic.h>
#include <driver/driver.h>
#include "uha/uha.h"

static bool init() {

    for (int i = 0; i < FDC_FDD_SUPPORT_COUNT; ++i) {
        perf[i] = (struct fdc_fddPerf){ FDC_HUT_DEFAULT, FDC_SRT_DEFAULT, FDC_HLT_DEFAULT };
    }

    /* Checking if controller present and supported */

    enum fdc_opStatus result = cmd_version(FDC_BUS_BASE);

    if (result != OPSTATUS_OK) {

        /* Controller not present (not ready) */

        present = false;
        driveCount = 0;
        OOPS("82077AA Driver initialization failed: controller not ready");
        return false;
    }

    uint_8 version;
    result = pullData(FDC_BUS_BASE, &version);
    
    if (result != OPSTATUS_OK) {
        
        /* Controller error */

        present = false;
        driveCount = 0;
        OOPS("82077AA Driver initialization failed: controller version check error");
        return false;
    }
    if (version != FDC_VERSION_82077AA) {
    
        /* Controller not supported */

        present = false;
        driveCount = 0;
        OOPS("82077AA Driver initialization failed: controller not supported");
        return false;
    }

    present = true;

    return true;
}

static bool start() {

    if (!present) {
        return false;
    }

    /* Restarting controller */

    enum fdc_opStatus s = proc_reset(FDC_BUS_BASE);
    if (s != OPSTATUS_OK) {
        proc_error(FDC_BUS_BASE, s, "Controller reset failed");
        return false;
    }

    /* Detecting drives */

    for (int i = 0; i < FDC_FDD_SUPPORT_COUNT; ++i) {
        
        uint_8 present;
        enum fdc_opStatus s = proc_fdcDetect(FDC_BUS_BASE, i, &present);
        if (s != OPSTATUS_OK && s != OPSTATUS_INVDRIVE) {
            proc_error(FDC_BUS_BASE, s, "Unexpected controller status during drive detection");
            return false;
        }

        driveCount += (present & 1) << i;
    }

    return true;
}

ISR(
    irqRecv = true;
)

static struct gnwDriverConfig desc() {
    return (struct gnwDriverConfig){ init, start, isr, 6 };
}

static struct gnwDeviceUHA uha() {
    struct gnwDeviceUHA uha;

    uha.storCtrl.routine.drivePresent = uha_drivePresent;
    uha.storCtrl.routine.driveGeometry = uha_driveGeometry;
    uha.storCtrl.routine.read = uha_read;
    uha.storCtrl.desc.driveCount = FDC_FDD_PER_CONTROLLER;
    uha.storCtrl.desc.removable = false;
    uha.storCtrl.desc.removableMedia = true;

    return uha;
}

struct gnwDeviceDescriptor k_drv_fdc_descriptor() {
    return (struct gnwDeviceDescriptor) {
        /* type */ DEV_TYPE_FDC,
        /* api */ uha(),
        /* driver */ (struct gnwDeviceDriver) {
            /* io */ (struct gnwDeviceIO) {
                /* busBase */ FDC_BUS_BASE
            },
            /* descriptor */ desc()
        },
        /* name */ "82077AA Floppy Disk Controller"
    };
}
