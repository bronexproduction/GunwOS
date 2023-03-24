//
//  diskr.c
//  GunwOS
//
//  Created by Artur Danielewski on 01.04.2020.
//

#include "../cmdutil.h"
#include "../../terminal/terminal.h"
#include "../../stor/manager/stormgr.h"

void cmd_diskr(const char * const params) {
    CMD_PARAM_INT(0, uint_8, diskID, "disk ID")
    CMD_PARAM_INT(1, uint_32, lba, "LBA address")

    const struct s_stor_drvDescriptor desc = s_stormgr_drive(diskID);

    if (!desc.meta.present) {
        s_trm_puts("Error: Disk with id ");
        s_trm_putun(diskID);
        s_trm_puts(" does not exist");
        return;
    }

    if (!desc.format.sectSize) {
        s_trm_puts("Error: Disk format error on drive ");
        s_trm_putun(diskID);
        return;
    }

    if (lba >= desc.format.lba) {
        s_trm_puts("Error: LBA address ");
        s_trm_putun(lba);
        s_trm_puts(" exceeds disk size of ");
        s_trm_putun(desc.format.lba);
        return;
    }

    uint_8 buffer[desc.format.sectSize];
    struct gnwStorError error;
    s_stormgr_drvRead(diskID, lba, buffer, desc.format.sectSize, &error);

    if (error.code != NONE) {
        s_trm_puts("Error: Read failed with code ");
        s_trm_putin(error.code);
        s_trm_puts(", internal code: ");
        s_trm_putin(error.internalCode);
        return;
    }

    s_trm_puts("Sector ");
    s_trm_putun(lba);
    s_trm_puts(" on disk ");
    s_trm_putun(diskID);
    s_trm_puts(": ");

    for (size_t i = 0; i < desc.format.sectSize; ++i) {
        s_trm_putun_h((uint_8)(*(buffer + i)));
        s_trm_putc(' ');
    }
}
