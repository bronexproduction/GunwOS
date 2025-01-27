//
//  defaults.c
//  GunwOS
//
//  Created by Artur Danielewski on 13.01.2023.
//

/*
    Initialization of default drivers
*/

#include "defaults.h"

#include <gunwdrv.h>
#include <gunwfilesys.h>
#include <gunwdev.h>

#include <dev/dev.h>
#include <hal/proc/proc.h>
#include <storage/filesys.h>
#include <error/panic.h>

#define MSG_INSTALL_FAIL(NAME) "Fatal error: " STR(NAME) " driver installation failed"
#define MSG_START_FAIL(NAME) "Fatal error: " STR(NAME) " driver startup failed"
#define MSGS_FAIL(NAME) MSG_INSTALL_FAIL(NAME), MSG_START_FAIL(NAME)

static void loadDevice(struct gnwDeviceDescriptor (*descProvider)(),
                       size_t * const deviceIdPtr,
                       const char * const installFailureMsg,
                       const char * const startFailureMsg) {
    if (!deviceIdPtr) {
        OOPS("Device identifier pointer nullptr",);
        return;
    }

    enum gnwDriverError e;
    const struct gnwDeviceDescriptor desc = descProvider();
    e = k_dev_install(&desc, deviceIdPtr);
    if (e != GDRE_NONE) { 
        OOPS(installFailureMsg,); 
    }

    e = k_dev_start(*(deviceIdPtr));
    if (e != GDRE_NONE) { 
        OOPS(startFailureMsg,);
    }
}

static void loadFileSystem(struct gnwFileSystemDescriptor (*descProvider)(),
                           const char * const installFailureMsg) {
    enum k_stor_error e;
    const struct gnwFileSystemDescriptor desc = descProvider();
    e = k_stor_fileSys_install(&desc);
    if (e != SE_NONE) { 
        OOPS(installFailureMsg,);
    }
}

void k_drv_loadMinimal() {

    /*
        PIT driver for 8253/8254 chip
    */
    extern struct gnwDeviceDescriptor k_drv_pit_descriptor();
    extern size_t k_drv_pit_deviceId;
    loadDevice(k_drv_pit_descriptor, &k_drv_pit_deviceId, MSGS_FAIL(PIT));

    /*
        Keyboard controller driver for 8042 PS/2 chip
    */
    extern struct gnwDeviceDescriptor k_drv_keyboard_descriptor();
    extern size_t k_drv_keyboard_deviceId;
    loadDevice(k_drv_keyboard_descriptor, &k_drv_keyboard_deviceId, MSGS_FAIL(Keyboard));

    /*
        82077AA Floppy disk controller
    */
    extern struct gnwDeviceDescriptor k_drv_fdc_descriptor();
    extern size_t k_drv_fdc_deviceId;
    loadDevice(k_drv_fdc_descriptor, &k_drv_fdc_deviceId, MSGS_FAIL(Floppy disk controller));

    /*
        FAT12 file system
    */
    extern struct gnwFileSystemDescriptor k_drv_fat12_descriptor();
    loadFileSystem(k_drv_fat12_descriptor, MSG_INSTALL_FAIL(FAT12 File system));
}
