
mod helpers;

use utils::*;
use kernel_symbols::*;
use self::helpers::*;

/*
    PRIVATE bool validateId(size_t id)
*/

#[test_case]
fn validateId_checkCorrect() {
    log("validateId_checkCorrect start\n\0");
    unsafe {
        assert_eq!(validateId(0), true);
        assert_eq!(validateId(MAX_DEVICES - 1), true);
    }
    log("validateId_checkCorrect end\n\0");
}

#[test_case]
fn validateId_checkIncorrect() {
    log("validateId_checkIncorrect start\n\0");
    unsafe {
        assert_eq!(validateId(MAX_DEVICES), false);
        assert_eq!(validateId(MAX_DEVICES + 1), false);
    }
    log("validateId_checkIncorrect end\n\0");
}

/*
    PRIVATE bool validateInstalledId(size_t id)
*/

#[test_case]
fn validateInstalledId_checkCorrect() {
    log("validateInstalledId_checkCorrect start\n\0");
    unsafe {
        devicesCount = MAX_DEVICES / 2;
        assert_eq!(validateInstalledId(0), true);
        assert_eq!(validateInstalledId(devicesCount - 1), true);
    }
    log("validateInstalledId_checkCorrect end\n\0");
}

#[test_case]
fn validateInstalledId_checkIncorrect() {
    log("validateInstalledId_checkIncorrect start\n\0");
    unsafe {
        devicesCount = MAX_DEVICES / 2;
        assert_eq!(validateInstalledId(devicesCount), false);
        assert_eq!(validateInstalledId(devicesCount + 1), false);
        assert_eq!(validateInstalledId(MAX_DEVICES - 1), false);
        assert_eq!(validateInstalledId(MAX_DEVICES), false);
        assert_eq!(validateInstalledId(MAX_DEVICES + 1), false);
    }
    log("validateInstalledId_checkIncorrect end\n\0");
}

/*
    PRIVATE enum gnwDeviceError validateStartedDevice(const procId_t processId, const size_t deviceId)
*/

#[test_case]
fn validateStartedDevice_checkCorrect() {
    log("validateStartedDevice_checkCorrect start\n\0");
    unsafe {
        devicesCount = 1;
        devices[0].holder = 0;
        devices[0].started = true;
        assert_eq!(validateStartedDevice(0, 0), gnwDeviceError::GDE_NONE);
    }
    log("validateStartedDevice_checkCorrect end\n\0");
}

#[test_case]
fn validateStartedDevice_checkIncorrect() {
    log("validateStartedDevice_checkIncorrect start\n\0");
    unsafe {
        devicesCount = 1;
        devices[0].holder = NONE_PROC_ID;
        devices[0].started = false;
        assert_eq!(validateStartedDevice(0, 1), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(validateStartedDevice(0, 0), gnwDeviceError::GDE_HANDLE_INVALID);
        devices[0].holder = 0;
        assert_eq!(validateStartedDevice(0, 0), gnwDeviceError::GDE_INVALID_DEVICE_STATE);
    }
    log("validateStartedDevice_checkIncorrect end\n\0");
}

/*
    enum gnwDriverError k_dev_install(size_t * const id, const struct gnwDeviceDescriptor * const descriptor)
*/

#[test_case]
fn k_dev_install_checkCorrect_simple() {
    log("k_dev_install_checkCorrect_simple start\n\0");

    let id: size_t = 0;
    let mut device_descriptor = create_empty_device_desc();
    device_descriptor.r#type = gnwDeviceType::DEV_TYPE_SYSTEM as i32;

    unsafe {
        assert_eq!(k_dev_install(&id, &device_descriptor), gnwDriverError::NO_ERROR);
    }

    log("k_dev_install_checkCorrect_simple end\n\0");
}

static mut DRIVER_INIT_CALLED: bool = false;

#[test_case]
fn k_dev_install_checkCorrect_complex() {
    log("k_dev_install_checkCorrect_complex start\n\0");

    let expected_device_id = MAX_DEVICES - 1;
    let expected_device_count = MAX_DEVICES;
    unsafe {
        devicesCount = expected_device_id;
    }

    let id: size_t = 0;
    let mut device_descriptor = create_empty_device_desc();
    unsafe {
        DRIVER_INIT_CALLED = false;
    }
    device_descriptor.r#type = gnwDeviceType::DEV_TYPE_SYSTEM as i32   |
                               gnwDeviceType::DEV_TYPE_MEM as i32      |
                               gnwDeviceType::DEV_TYPE_KEYBOARD as i32 |
                               gnwDeviceType::DEV_TYPE_MOUSE as i32    |
                               gnwDeviceType::DEV_TYPE_DISPLAY as i32  |
                               gnwDeviceType::DEV_TYPE_CHAR_IN as i32  |
                               gnwDeviceType::DEV_TYPE_CHAR_OUT as i32 |
                               gnwDeviceType::DEV_TYPE_STORAGE as i32  |
                               gnwDeviceType::DEV_TYPE_FDC as i32;

    extern "C" fn system_get_param(_: u32, _: u32, _: u32, _: *mut u32) -> bool { return false; }
    device_descriptor.api.system.routine.getParam = Some(system_get_param);
    extern "C" fn system_set_param(_: u32, _: u32, _: u32, _: u32) -> bool { return false; }
    device_descriptor.api.system.routine.setParam = Some(system_set_param);

    device_descriptor.api.mem.desc.bytesRange = range_addr_t {
        offset: 0x69,
        sizeBytes: 0x69,
    };
    extern "C" fn mem_write(_: *mut u8, _: range_addr_t) {}
    device_descriptor.api.mem.routine.write = Some(mem_write);

    device_descriptor.api.display.desc.supportedFormatCount = 1;

    extern "C" fn char_in_has_data() -> bool { return false; }
    device_descriptor.api.charIn.routine.hasData = Some(char_in_has_data);
    extern "C" fn char_in_read(_: *const u8) -> u32 { return 0; }
    device_descriptor.api.charIn.routine.read = Some(char_in_read);

    extern "C" fn char_out_is_ready_to_write() -> bool { return false; }
    device_descriptor.api.charOut.routine.isReadyToWrite = Some(char_out_is_ready_to_write);
    extern "C" fn char_out_write(_: i8) -> bool { return false; }
    device_descriptor.api.charOut.routine.write = Some(char_out_write);

    device_descriptor.api.storCtrl.desc.driveCount = 1;
    extern "C" fn stor_ctrl_drive_geometry(_: u8) -> gnwStorGeometry { 
        return gnwStorGeometry {
            chs: gnwStorCHS {
                c: 0,
                h: 0,
                s: 0,
            },
            lba: 0,
            sectSizeBytes: 0,
        };
    }
    device_descriptor.api.storCtrl.routine.driveGeometry = Some(stor_ctrl_drive_geometry);
    extern "C" fn stor_ctrl_drive_present(_: u8) -> bool { return false; }
    device_descriptor.api.storCtrl.routine.drivePresent = Some(stor_ctrl_drive_present);
    extern "C" fn stor_ctrl_read(_: u8, _: u32, _: u32, _: *const u8, _: *mut gnwStorError) -> u32 { return 0; }
    device_descriptor.api.storCtrl.routine.read = Some(stor_ctrl_read);

    device_descriptor.driver.descriptor.irq = DEV_IRQ_LIMIT - 1;
    
    // ISR has to be disabled due to k_pic_enableIRQ causing triple fault
    // extern "C" fn isr() {}
    // device_descriptor.driver.descriptor.isr = Some(isr);
    device_descriptor.driver.descriptor.isr = None;
    extern "C" fn init() -> bool {
        unsafe {
            DRIVER_INIT_CALLED = true;
        }
        return true;
    }
    device_descriptor.driver.descriptor.init = Some(init);

    unsafe {
        /*
            Check install
        */
        assert_eq!(k_dev_install(&id, &device_descriptor), gnwDriverError::NO_ERROR);
        
        /*
            Check driver init called
        */
        assert_eq!(DRIVER_INIT_CALLED, true);
        
        /*
            Check device count
        */
        assert_eq!(devicesCount, expected_device_count);
        
        /*
            Check devices array
        */
        assert_eq!(devices[id as usize].desc, device_descriptor);
        assert_eq!(devices[id as usize].initialized, true);
        assert_eq!(devices[id as usize].started, false);
        assert_eq!(devices[id as usize].holder, NONE_PROC_ID);
        assert_eq!(devices[id as usize].listener, None);
        assert_eq!(devices[id as usize].decoder, None);
    }

    /*
        Check device identifier
    */    
    assert_eq!(id, expected_device_id);
    
    // check k_hal_install outcome - not applicable as long as ISR check is disabled    
    
    log("k_dev_install_checkCorrect_complex end\n\0");
}

#[test_case]
fn k_dev_install_checkIncorrect_idNull() {
    log("k_dev_install_checkIncorrect_idNull start\n\0");

    unsafe {
        assert_eq!(k_dev_install(core::ptr::null(), core::ptr::null()), gnwDriverError::UNKNOWN);
    }

    log("k_dev_install_checkIncorrect_idNull end\n\0");
}
