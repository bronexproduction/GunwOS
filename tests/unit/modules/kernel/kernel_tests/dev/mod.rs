
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
    let device_descriptor = create_valid_device_desc_minimal();

    unsafe {
        assert_eq!(k_dev_install(&id, &device_descriptor), gnwDriverError::NO_ERROR);
    }

    log("k_dev_install_checkCorrect_simple end\n\0");
}

#[test_case]
fn k_dev_install_checkCorrect_complex() {
    log("k_dev_install_checkCorrect_complex start\n\0");

    let expected_device_id = MAX_DEVICES - 1;
    let expected_device_count = MAX_DEVICES;
    unsafe {
        devicesCount = expected_device_id;
    }

    let id: size_t = 0;
    let device_descriptor = create_valid_device_desc_complex();
    unsafe {
        DRIVER_INIT_CALLED = false;
    }
    
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

#[test_case]
fn k_dev_install_checkIncorrect_deviceLimitReached() {
    log("k_dev_install_checkIncorrect_deviceLimitReached start\n\0");

    let id: size_t = 0;
    unsafe {
        devicesCount = MAX_DEVICES;
        assert_eq!(k_dev_install(&id, core::ptr::null()), gnwDriverError::LIMIT_REACHED);
        devicesCount = MAX_DEVICES + 1;
        assert_eq!(k_dev_install(&id, core::ptr::null()), gnwDriverError::LIMIT_REACHED);
    }

    log("k_dev_install_checkIncorrect_deviceLimitReached end\n\0");
}

#[test_case]
fn k_dev_install_checkIncorrect_nullDescriptor() {
    log("k_dev_install_checkIncorrect_nullDescriptor start\n\0");

    let id: size_t = 0;
    unsafe {
        assert_eq!(k_dev_install(&id, core::ptr::null()), gnwDriverError::UNKNOWN);
    }

    log("k_dev_install_checkIncorrect_nullDescriptor end\n\0");
}

#[test_case]
fn k_dev_install_checkIncorrect_invalidDescriptor() {
    log("k_dev_install_checkIncorrect_invalidDescriptor start\n\0");

    let id: size_t = 0;
    let device_descriptor = create_empty_device_desc();
    unsafe {
        assert_eq!(k_dev_install(&id, &device_descriptor), gnwDriverError::UNKNOWN);
    }

    log("k_dev_install_checkIncorrect_invalidDescriptor end\n\0");
}

#[test_case]
fn k_dev_install_checkIncorrect_invalidIrq() {
    log("k_dev_install_checkIncorrect_invalidIrq start\n\0");

    let id: size_t = 0;
    let mut device_descriptor = create_valid_device_desc_minimal();
    device_descriptor.driver.descriptor.irq = DEV_IRQ_LIMIT;
    unsafe {
        assert_eq!(k_dev_install(&id, &device_descriptor), gnwDriverError::IRQ_INVALID);
    }
    device_descriptor.driver.descriptor.irq = DEV_IRQ_LIMIT + 1;
    unsafe {
        assert_eq!(k_dev_install(&id, &device_descriptor), gnwDriverError::IRQ_INVALID);
    }

    log("k_dev_install_checkIncorrect_invalidIrq end\n\0");
}

#[test_case]
fn k_dev_install_checkIncorrect_irqConflict() {
    log("k_dev_install_checkIncorrect_irqConflict start\n\0");

    let id: size_t = 0;
    let mut device_descriptor = create_valid_device_desc_minimal();
    extern "C" fn isr() {}
    device_descriptor.driver.descriptor.isr = Some(isr);

    unsafe {
        isrReg[0].routine = Some(isr);
        assert_eq!(k_dev_install(&id, &device_descriptor), gnwDriverError::IRQ_CONFLICT);
    }
    
    log("k_dev_install_checkIncorrect_irqConflict end\n\0");
}
