//
//  setup.c
//  GunwOS
//
//  Created by Artur Danielewski on 25.03.2020.
//  
//  Docs:
//  * Intel 82077AA Datasheet
//  * https://wiki.osdev.org/Floppy_Disk_Controller
//

#include "../../../../../../lib/stdgunw/types.h"
#include "../shared/defaults.h"
#include "../shared/types.h"
#include "../../../driver.h"
#include "../../../../../include/gunwio.h"
#include "../../../../../include/gunwdrv.h"
#include "../../../../../include/gunwstor.h"
#include "../../../../shared/log/log.h"
#include "../shared/io.h"

static uint_8 init() {

    extern struct fdc_fddPerf *fdc_perf;
    extern uint_16 fdc_busBase;
    extern uint_8 fdc_present;
    extern uint_8 fdc_drives;

    for (int i = 0; i < FDC_FDD_SUPPORT_COUNT; ++i) {
        fdc_perf[i] = (struct fdc_fddPerf){ FDC_HUT_DEFAULT, FDC_SRT_DEFAULT, FDC_HLT_DEFAULT };
    }

    // Checking if controller present and supported
    enum k_fdc_opStatus result = fdc_cmd_version(fdc_busBase);

    if (result != OPSTATUS_OK) {    // Controller not present (not ready)
        fdc_present = fdc_drives = 0;
        LOG_INFO("82077AA Driver initialization failed: controller not ready")
        return 0;
    }

    uint_8 version;
    result = pullData(fdc_busBase, &version);
    
    if (result != OPSTATUS_OK) {    // Controller error
        fdc_present = fdc_drives = 0;
        LOG_INFO("82077AA Driver initialization failed: controller version check error")
        return 0;
    }
    if (version != FDC_VERSION_82077AA) {   // Controller not supported
        fdc_present = fdc_drives = 0;
        LOG_INFO("82077AA Driver initialization failed: controller not supported")
        return 0;
    }

    // Detected supported controller - marking as present
    fdc_present = 1;

    return 1;
}

static uint_8 start() {
    
    // Restarting controller
    extern uint_16 fdc_busBase;
    extern uint_8 fdc_present;

    if (fdc_present) {
        extern enum k_fdc_opStatus reset(const uint_16 base);
        enum k_fdc_opStatus s = reset(fdc_busBase);
        if (s != OPSTATUS_OK) {
            fdc_proc_error(fdc_busBase, s, "Controller reset failed");
            return 0;
        }

        extern uint_8 fdc_drives;
        for (int i = 0; i < FDC_FDD_SUPPORT_COUNT; ++i) {
            
            uint_8 present;
            enum k_fdc_opStatus s = fdc_proc_fdcDetect(fdc_busBase, i, &present);
            if (s != OPSTATUS_OK && s != OPSTATUS_INVDRIVE) {
                fdc_proc_error(fdc_busBase, s, "Unexpected controller status during drive detection");
                return 0;
            }

            fdc_drives += (present & 1) << i;
        }
    }

    return 1;
}

ISR(
    extern uint_8 fdc_irqRecv;
    fdc_irqRecv = 1;
)

struct gnwDriverDesc s_drv_fdc() {
    return (struct gnwDriverDesc){ init, start, isr, 6 };
}

extern uint_8 s_drv_fdc_uha_drivePresent(const uint_8);
extern struct gnwStorGeometry s_drv_fdc_uha_driveGeometry(const uint_8);
extern size_t s_drv_fdc_uha_read(const uint_8 index, const size_t lba, const size_t count, uint_8 * const buffer, struct gnwStorError *error);

const struct gnwDeviceUHA s_drv_fdc_uha() {
    struct gnwDeviceUHA uha = { 0 };

    uha.fdc.drivePresent = s_drv_fdc_uha_drivePresent;
    uha.fdc.driveGeometry = s_drv_fdc_uha_driveGeometry;
    
    uha.storage.read = s_drv_fdc_uha_read;

    return uha;
}
