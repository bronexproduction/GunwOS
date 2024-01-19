pub mod helpers;

use utils::*;
use kernel_symbols::*;
use self::helpers::*;
use core::ptr::null;
use core::ptr::null_mut;

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
        install_dummy_process();
        assert_eq!(validateStartedDevice(0, 0), gnwDeviceError::GDE_NONE);
    }
    log("validateStartedDevice_checkCorrect end\n\0");
}

#[test_case]
fn validateStartedDevice_checkIncorrect() {
    log("validateStartedDevice_checkIncorrect start\n\0");
    unsafe {
        devicesCount = 1;
        let proc_id = install_dummy_process();
        assert_eq!(proc_id, 0);
        assert_eq!(validateStartedDevice(NONE_PROC_ID - 1, 0), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(validateStartedDevice(NONE_PROC_ID, 0), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(validateStartedDevice(KERNEL_PROC_ID, 0), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(validateStartedDevice(1, 0), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(validateStartedDevice(MAX_PROC - 1, 0), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(validateStartedDevice(MAX_PROC, 0), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(validateStartedDevice(MAX_PROC + 1, 0), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        
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
        assert_eq!(k_dev_install(&id, &device_descriptor), gnwDriverError::GDRE_NONE);
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
        assert_eq!(k_dev_install(&id, &device_descriptor), gnwDriverError::GDRE_NONE);
        
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
        assert_eq!(k_dev_install(core::ptr::null(), core::ptr::null()), gnwDriverError::GDRE_UNKNOWN);
    }

    log("k_dev_install_checkIncorrect_idNull end\n\0");
}

#[test_case]
fn k_dev_install_checkIncorrect_deviceLimitReached() {
    log("k_dev_install_checkIncorrect_deviceLimitReached start\n\0");

    let id: size_t = 0;
    unsafe {
        devicesCount = MAX_DEVICES;
        assert_eq!(k_dev_install(&id, core::ptr::null()), gnwDriverError::GDRE_LIMIT_REACHED);
        devicesCount = MAX_DEVICES + 1;
        assert_eq!(k_dev_install(&id, core::ptr::null()), gnwDriverError::GDRE_LIMIT_REACHED);
    }

    log("k_dev_install_checkIncorrect_deviceLimitReached end\n\0");
}

#[test_case]
fn k_dev_install_checkIncorrect_nullDescriptor() {
    log("k_dev_install_checkIncorrect_nullDescriptor start\n\0");

    let id: size_t = 0;
    unsafe {
        assert_eq!(k_dev_install(&id, core::ptr::null()), gnwDriverError::GDRE_INVALID_DESCRIPTOR);
    }

    log("k_dev_install_checkIncorrect_nullDescriptor end\n\0");
}

#[test_case]
fn k_dev_install_checkIncorrect_invalidDescriptor() {
    log("k_dev_install_checkIncorrect_invalidDescriptor start\n\0");

    let id: size_t = 0;
    let device_descriptor = create_empty_device_desc();
    unsafe {
        assert_eq!(k_dev_install(&id, &device_descriptor), gnwDriverError::GDRE_INVALID_DESCRIPTOR);
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
        assert_eq!(k_dev_install(&id, &device_descriptor), gnwDriverError::GDRE_IRQ_INVALID);
    }
    device_descriptor.driver.descriptor.irq = DEV_IRQ_LIMIT + 1;
    unsafe {
        assert_eq!(k_dev_install(&id, &device_descriptor), gnwDriverError::GDRE_IRQ_INVALID);
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
        assert_eq!(k_dev_install(&id, &device_descriptor), gnwDriverError::GDRE_IRQ_CONFLICT);
    }
    
    log("k_dev_install_checkIncorrect_irqConflict end\n\0");
}

#[test_case]
fn k_dev_install_checkIncorrect_initFailure() {
    log("k_dev_install_checkIncorrect_initFailure start\n\0");

    let id: size_t = 0;
    let mut device_descriptor = create_valid_device_desc_minimal();
    extern "C" fn init() -> bool { return false; }
    device_descriptor.driver.descriptor.init = Some(init);

    unsafe {
        assert_eq!(k_dev_install(&id, &device_descriptor), gnwDriverError::GDRE_UNINITIALIZED);
    }
    
    log("k_dev_install_checkIncorrect_initFailure end\n\0");
}

/*
    enum gnwDriverError k_dev_start(size_t id)
*/

#[test_case]
fn k_dev_start_checkCorrect() {
    log("k_dev_start_checkCorrect start\n\0");

    let id: size_t = 0;
    install_dummy_device(&id, false);

    unsafe {
        assert_eq!(k_dev_start(id), gnwDriverError::GDRE_NONE);
    }
    
    log("k_dev_start_checkCorrect end\n\0");
}

#[test_case]
fn k_dev_start_checkIncorrect_idInvalid() {
    log("k_dev_start_checkIncorrect_idInvalid start\n\0");

    unsafe {
        assert_eq!(k_dev_start(MAX_DEVICES), gnwDriverError::GDRE_UNKNOWN);
        assert_eq!(k_dev_start(MAX_DEVICES + 1), gnwDriverError::GDRE_UNKNOWN);
    }
    
    log("k_dev_start_checkIncorrect_idInvalid end\n\0");
}

#[test_case]
fn k_dev_start_checkIncorrect_deviceNotInitialized() {
    log("k_dev_start_checkIncorrect_deviceNotInitialized start\n\0");

    let id: size_t = 0;
    install_dummy_device(&id, false);

    unsafe {
        devices[0].initialized = false;
        k_dev_start(0);
        assert_eq!(KERNEL_PANIC_FLAG, true);
    }
    
    log("k_dev_start_checkIncorrect_deviceNotInitialized end\n\0");
}

#[test_case]
fn k_dev_start_checkIncorrect_deviceStartFailed() {
    log("k_dev_start_checkIncorrect_deviceStartFailed start\n\0");

    let id: size_t = 0;
    install_dummy_device(&id, false);

    extern "C" fn start() -> bool { return false; }
    unsafe {
        devices[0].desc.driver.descriptor.start = Some(start);
        assert_eq!(k_dev_start(0), gnwDriverError::GDRE_START_FAILED);
    }
    
    log("k_dev_start_checkIncorrect_deviceStartFailed end\n\0");
}

/*
    enum gnwDeviceError k_dev_getById(const size_t id, struct gnwDeviceUHADesc * const desc)
*/

#[test_case]
fn k_dev_getById_checkCorrect() {
    log("k_dev_getById_checkCorrect start\n\0");

    let id: size_t = 0;
    let uha_descriptor: gnwDeviceUHADesc = Default::default();
    let expected_descriptor = create_valid_device_desc_complex();
    install_device(&id, expected_descriptor);
    assert_eq!(id, 0);
    
    unsafe {
        let expected_api_desc = uhaGetDesc(id, expected_descriptor.r#type, expected_descriptor.api);
        assert_eq!(k_dev_getById(id, &uha_descriptor), gnwDeviceError::GDE_NONE);
        assert_eq!(uha_descriptor, expected_api_desc);
    }
    
    log("k_dev_getById_checkCorrect end\n\0");
}

#[test_case]
fn k_dev_getById_checkIncorrect_idInvalid() {
    log("k_dev_getById_checkIncorrect_idInvalid start\n\0");

    let mut id: size_t = 0;
    let uha_descriptor: gnwDeviceUHADesc = Default::default();
    unsafe {
        assert_eq!(k_dev_getById(id, &uha_descriptor), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
    }
    install_dummy_device(&id, true);
    assert_eq!(id, 0);
    id = 1;
    unsafe {
        assert_eq!(k_dev_getById(id, &uha_descriptor), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
    }
    id = MAX_DEVICES;
    unsafe {
        assert_eq!(k_dev_getById(id, &uha_descriptor), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
    }
    
    log("k_dev_getById_checkIncorrect_idInvalid end\n\0");
}

#[test_case]
fn k_dev_getById_checkIncorrect_descNull() {
    log("k_dev_getById_checkIncorrect_descNull start\n\0");

    let id: size_t = 0;
    install_dummy_device(&id, true);
    assert_eq!(id, 0);
    unsafe {
        assert_eq!(k_dev_getById(id, null()), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
    }
    
    log("k_dev_getById_checkIncorrect_descNull end\n\0");
}

/*
    enum gnwDeviceError k_dev_getByType(const enum gnwDeviceType type, struct gnwDeviceUHADesc * const desc)
*/

#[test_case]
fn k_dev_getByType_checkCorrect_minimal() {
    log("k_dev_getByType_checkCorrect_minimal start\n\0");

    let id: size_t = 0;
    let uha_descriptor: gnwDeviceUHADesc = Default::default();
    let expected_descriptor = create_valid_device_desc_minimal();
    install_device(&id, expected_descriptor);
    unsafe {
        let expected_api_desc = uhaGetDesc(id, expected_descriptor.r#type, expected_descriptor.api);
        assert_eq!(k_dev_getByType(gnwDeviceType::DEV_TYPE_FDC as u32, &uha_descriptor), gnwDeviceError::GDE_NOT_FOUND);
        assert_eq!(k_dev_getByType(gnwDeviceType::DEV_TYPE_MEM as u32, &uha_descriptor), gnwDeviceError::GDE_NOT_FOUND);
        assert_eq!(k_dev_getByType(gnwDeviceType::DEV_TYPE_SYSTEM as u32, &uha_descriptor), gnwDeviceError::GDE_NONE);
        assert_eq!(uha_descriptor, expected_api_desc);
    }
    
    log("k_dev_getByType_checkCorrect_minimal end\n\0");
}

#[test_case]
fn k_dev_getByType_checkCorrect_complex() {
    log("k_dev_getByType_checkCorrect_complex start\n\0");

    let id: size_t = 0;
    let uha_descriptor: gnwDeviceUHADesc = Default::default();
    let expected_descriptor = create_valid_device_desc_complex();
    install_device(&id, expected_descriptor);
    unsafe {
        let expected_api_desc = uhaGetDesc(id, expected_descriptor.r#type, expected_descriptor.api);
        assert_eq!(k_dev_getByType(gnwDeviceType::DEV_TYPE_FDC as u32, &uha_descriptor), gnwDeviceError::GDE_NONE);
        assert_eq!(uha_descriptor, expected_api_desc);
        assert_eq!(k_dev_getByType(gnwDeviceType::DEV_TYPE_MEM as u32, &uha_descriptor), gnwDeviceError::GDE_NONE);
        assert_eq!(uha_descriptor, expected_api_desc);
        assert_eq!(k_dev_getByType(gnwDeviceType::DEV_TYPE_SYSTEM as u32, &uha_descriptor), gnwDeviceError::GDE_NONE);
        assert_eq!(uha_descriptor, expected_api_desc);
    }
    
    log("k_dev_getByType_checkCorrect_complex end\n\0");
}

#[test_case]
fn k_dev_getByType_checkIncorrect_descNull() {
    log("k_dev_getByType_checkIncorrect_descNull start\n\0");

    unsafe {
        assert_eq!(k_dev_getByType(gnwDeviceType::DEV_TYPE_SYSTEM as u32, null()), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
    }
    
    log("k_dev_getByType_checkIncorrect_descNull end\n\0");
}

/*
    enum gnwDeviceError k_dev_getUHAForId(const size_t id, struct gnwDeviceUHA * const uha)
*/

#[test_case]
fn k_dev_getUHAForId_checkCorrect() {
    log("k_dev_getUHAForId_checkCorrect start\n\0");

    let id: size_t = 0;
    let uha: gnwDeviceUHA = Default::default();
    let expected_descriptor = create_valid_device_desc_complex();
    install_device(&id, expected_descriptor);
    assert_eq!(id, 0);
    unsafe {
        let expected_uha = expected_descriptor.api;
        assert_eq!(k_dev_getUHAForId(id, &uha), gnwDeviceError::GDE_NONE);
        assert_eq!(uha, expected_uha);
    }
    
    log("k_dev_getUHAForId_checkCorrect end\n\0");
}

#[test_case]
fn k_dev_getUHAForId_checkIncorrect_idInvalid() {
    log("k_dev_getUHAForId_checkIncorrect_idInvalid start\n\0");

    let mut id: size_t = 0;
    let uha: gnwDeviceUHA = Default::default();
    unsafe {
        assert_eq!(k_dev_getUHAForId(id, &uha), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
    }
    let descriptor = create_valid_device_desc_complex();
    install_device(&id, descriptor);
    id = 1;
    unsafe {
        assert_eq!(k_dev_getUHAForId(id, &uha), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
    }
    id = MAX_DEVICES;
    unsafe {
        assert_eq!(k_dev_getUHAForId(id, &uha), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
    }
    
    log("k_dev_getUHAForId_checkIncorrect_idInvalid end\n\0");
}

#[test_case]
fn k_dev_getUHAForId_checkIncorrect_descNull() {
    log("k_dev_getUHAForId_checkIncorrect_descNull start\n\0");

    unsafe {
        assert_eq!(k_dev_getUHAForId(0, null()), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
    }
    
    log("k_dev_getUHAForId_checkIncorrect_descNull end\n\0");
}

/*
    enum gnwDeviceError k_dev_acquireHold(const procId_t processId, const size_t deviceId)
*/

#[test_case]
fn k_dev_acquireHold_checkCorrect() {
    log("k_dev_acquireHold_checkCorrect start\n\0");

    let id: size_t = 0;
    let proc_id: procId_t = install_dummy_process();
    install_dummy_device(&id, true);
    assert_eq!(id, 0);
    assert_eq!(proc_id, 0);

    unsafe {
        assert_eq!(k_dev_acquireHold(proc_id, id), gnwDeviceError::GDE_NONE);
        assert_eq!(devices[id as usize].holder, proc_id);
    }
    
    log("k_dev_acquireHold_checkCorrect end\n\0");
}

#[test_case]
fn k_dev_acquireHold_checkIncorrect_deviceIdInvalid() {
    log("k_dev_acquireHold_checkIncorrect_deviceIdInvalid start\n\0");

    unsafe {
        assert_eq!(k_dev_acquireHold(0, 0), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(k_dev_acquireHold(0, 1), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(k_dev_acquireHold(0, MAX_DEVICES - 1), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(k_dev_acquireHold(0, MAX_DEVICES), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(k_dev_acquireHold(0, MAX_DEVICES + 1), gnwDeviceError::GDE_UNKNOWN);
    }
    let id: size_t = 0;
    install_dummy_device(&id, true);
    assert_eq!(id, 0);
    unsafe {
        assert_eq!(k_dev_acquireHold(0, 1), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(k_dev_acquireHold(0, MAX_DEVICES - 1), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(k_dev_acquireHold(0, MAX_DEVICES), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(k_dev_acquireHold(0, MAX_DEVICES + 1), gnwDeviceError::GDE_UNKNOWN);
    }

    log("k_dev_acquireHold_checkIncorrect_deviceIdInvalid end\n\0");
}

#[test_case]
fn k_dev_acquireHold_checkIncorrect_processIdInvalid() {
    log("k_dev_acquireHold_checkIncorrect_processIdInvalid start\n\0");

    let id: size_t = 0;
    install_dummy_device(&id, true);
    assert_eq!(id, 0);
    unsafe {
        assert_eq!(k_dev_acquireHold(NONE_PROC_ID - 1, id), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(k_dev_acquireHold(NONE_PROC_ID, id), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(k_dev_acquireHold(KERNEL_PROC_ID, id), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(k_dev_acquireHold(0, id), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(k_dev_acquireHold(1, id), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(k_dev_acquireHold(MAX_PROC - 1, id), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(k_dev_acquireHold(MAX_PROC, id), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(k_dev_acquireHold(MAX_PROC + 1, id), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
    }
    let proc_id = install_dummy_process();
    assert_eq!(proc_id, 0);
    unsafe {
        assert_eq!(k_dev_acquireHold(NONE_PROC_ID - 1, id), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(k_dev_acquireHold(NONE_PROC_ID, id), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(k_dev_acquireHold(KERNEL_PROC_ID, id), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(k_dev_acquireHold(1, id), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(k_dev_acquireHold(MAX_PROC - 1, id), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(k_dev_acquireHold(MAX_PROC, id), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(k_dev_acquireHold(MAX_PROC + 1, id), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
    }

    log("k_dev_acquireHold_checkIncorrect_processIdInvalid end\n\0");
}

#[test_case]
fn k_dev_acquireHold_checkIncorrect_alreadyHeld() {
    log("k_dev_acquireHold_checkIncorrect_alreadyHeld start\n\0");

    let id: size_t = 0;
    let proc_id: procId_t = install_dummy_process();
    install_dummy_device(&id, true);
    assert_eq!(id, 0);
    assert_eq!(proc_id, 0);
    unsafe {
        devices[id as usize].holder = 0;
        assert_eq!(k_dev_acquireHold(proc_id, id), gnwDeviceError::GDE_ALREADY_HELD);
        devices[id as usize].holder = 1;
        assert_eq!(k_dev_acquireHold(proc_id, id), gnwDeviceError::GDE_ALREADY_HELD);
    }
    
    log("k_dev_acquireHold_checkIncorrect_alreadyHeld end\n\0");
}

/*
    void k_dev_releaseHold(const procId_t processId, const size_t deviceId)
*/

#[test_case]
fn k_dev_releaseHold_checkCorrect() {
    log("k_dev_releaseHold_checkCorrect start\n\0");

    let id: size_t = 0;
    let proc_id: procId_t = install_dummy_process();
    install_dummy_device(&id, true);
    assert_eq!(id, 0);
    assert_eq!(proc_id, 0);
    install_dummy_device_holder(id, proc_id);
    unsafe {
        k_dev_releaseHold(proc_id, id);
        assert_eq!(KERNEL_PANIC_FLAG, false);
        assert_eq!(devices[id as usize].holder, NONE_PROC_ID);
        assert_eq!(devices[id as usize].listener, None);
        assert_eq!(devices[id as usize].decoder, None);
    }
    
    log("k_dev_releaseHold_checkCorrect end\n\0");
}

#[test_case]
fn k_dev_releaseHold_checkIncorrect_deviceIdInvalid() {
    log("k_dev_releaseHold_checkIncorrect_deviceIdInvalid start\n\0");

    unsafe {
        k_dev_releaseHold(0, 0);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        k_dev_releaseHold(0, 1);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        k_dev_releaseHold(0, MAX_DEVICES - 1);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        k_dev_releaseHold(0, MAX_DEVICES);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        k_dev_releaseHold(0, MAX_DEVICES + 1);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
    }
    let id: size_t = 0;
    install_dummy_device(&id, true);
    assert_eq!(id, 0);
    unsafe {
        k_dev_releaseHold(0, 1);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        k_dev_releaseHold(0, MAX_DEVICES - 1);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        k_dev_releaseHold(0, MAX_DEVICES);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        k_dev_releaseHold(0, MAX_DEVICES + 1);
        assert_eq!(KERNEL_PANIC_FLAG, true);
    }
    
    log("k_dev_releaseHold_checkIncorrect_deviceIdInvalid end\n\0");
}

#[test_case]
fn k_dev_releaseHold_checkIncorrect_processIdInvalid() {
    log("k_dev_releaseHold_checkIncorrect_deviceIdInvalid start\n\0");
    
    let id: size_t = 0;
    let proc_id = install_dummy_process();
    install_dummy_device(&id, true);
    assert_eq!(id, 0);
    assert_eq!(proc_id, 0);
    install_dummy_device_holder(id, proc_id);
    unsafe {
        let expected_device = devices[0];
        k_dev_releaseHold(NONE_PROC_ID - 1, 0);
        assert_eq!(devices[0], expected_device);
        k_dev_releaseHold(NONE_PROC_ID, 0);
        assert_eq!(devices[0], expected_device);
        k_dev_releaseHold(KERNEL_PROC_ID, 0);
        assert_eq!(devices[0], expected_device);
        k_dev_releaseHold(1, 0);
        assert_eq!(devices[0], expected_device);
        k_dev_releaseHold(MAX_PROC - 1, 0);
        assert_eq!(devices[0], expected_device);
        k_dev_releaseHold(MAX_PROC, 0);
        assert_eq!(devices[0], expected_device);
        k_dev_releaseHold(MAX_PROC + 1, 0);
        assert_eq!(devices[0], expected_device);
    }
    
    log("k_dev_releaseHold_checkIncorrect_deviceIdInvalid end\n\0");
}

/*
    enum gnwDeviceError k_dev_writeMem(const procId_t processId, 
                                       const size_t deviceId,
                                       const ptr_t absBuffer,
                                       const range_addr_t devMemRange)
*/

#[test_case]
fn k_dev_writeMem_checkCorrect() {
    log("k_dev_writeMem_checkCorrect start\n\0");

    let id: size_t = 0;
    let mut proc_id: procId_t = 0;
    install_dummy_writable_device(&id, &mut proc_id);
    let mut buffer: u8 = 0;
    let input_range = range_addr_t {
        offset: 0,
        sizeBytes: 1,
    };

    unsafe {
        devices[0].started = true;
        assert_eq!(k_dev_writeMem(proc_id, id, &mut buffer, input_range), gnwDeviceError::GDE_NONE);
        assert_eq!(DEV_WRITE_CALLED, true);
        DEV_WRITE_CALLED = false;
    }
    
    log("k_dev_writeMem_checkCorrect end\n\0");
}

#[test_case]
fn k_dev_writeMem_checkIncorrect_nullBuffer() {
    log("k_dev_writeMem_checkIncorrect_nullBuffer start\n\0");

    let id: size_t = 0;
    let mut proc_id: procId_t = 0;
    install_dummy_writable_device(&id, &mut proc_id);
    let input_range = range_addr_t {
        offset: 0,
        sizeBytes: 1,
    };

    unsafe {
        assert_eq!(k_dev_writeMem(proc_id, id, null_mut(), input_range), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
    }
    
    log("k_dev_writeMem_checkIncorrect_nullBuffer end\n\0");
}

#[test_case]
fn k_dev_writeMem_checkIncorrect_deviceNotInstalled() {
    log("k_dev_writeMem_checkIncorrect_deviceNotInstalled start\n\0");
    
    let id: size_t = 0;
    let mut proc_id: procId_t = 0;
    install_dummy_writable_device(&id, &mut proc_id);
    let mut buffer: u8 = 0;
    let input_range = range_addr_t {
        offset: 0,
        sizeBytes: 1,
    };

    unsafe {
        devicesCount = 0;
        assert_eq!(k_dev_writeMem(proc_id, id, &mut buffer, input_range), gnwDeviceError::GDE_UNKNOWN);
    }
    
    log("k_dev_writeMem_checkIncorrect_deviceNotInstalled end\n\0");
}

#[test_case]
fn k_dev_writeMem_checkIncorrect_deviceIdInvalid() {
    log("k_dev_writeMem_checkIncorrect_deviceIdInvalid start\n\0");
    
    let id: size_t = 0;
    let mut proc_id: procId_t = 0;
    install_dummy_writable_device(&id, &mut proc_id);
    let mut buffer: u8 = 0;
    let input_range = range_addr_t {
        offset: 0,
        sizeBytes: 1,
    };

    unsafe {
        assert_eq!(k_dev_writeMem(proc_id, 1, &mut buffer, input_range), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(k_dev_writeMem(proc_id, MAX_DEVICES - 1, &mut buffer, input_range), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(k_dev_writeMem(proc_id, MAX_DEVICES, &mut buffer, input_range), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(k_dev_writeMem(proc_id, MAX_DEVICES + 1, &mut buffer, input_range), gnwDeviceError::GDE_UNKNOWN);
    }
    
    log("k_dev_writeMem_checkIncorrect_deviceIdInvalid end\n\0");
}

#[test_case]
fn k_dev_writeMem_checkIncorrect_processIdInvalid() {
    log("k_dev_writeMem_checkIncorrect_processIdInvalid start\n\0");

    let id: size_t = 0;
    let mut proc_id: procId_t = 0;
    install_dummy_writable_device(&id, &mut proc_id);
    let mut buffer: u8 = 0;
    let input_range = range_addr_t {
        offset: 0,
        sizeBytes: 1,
    };

    unsafe {
        assert_eq!(k_dev_writeMem(NONE_PROC_ID - 1, id, &mut buffer, input_range), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(k_dev_writeMem(NONE_PROC_ID, id, &mut buffer, input_range), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(k_dev_writeMem(KERNEL_PROC_ID, id, &mut buffer, input_range), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(k_dev_writeMem(1, id, &mut buffer, input_range), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(k_dev_writeMem(MAX_PROC - 1, id, &mut buffer, input_range), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(k_dev_writeMem(MAX_PROC, id, &mut buffer, input_range), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(k_dev_writeMem(MAX_PROC + 1, id, &mut buffer, input_range), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
    }
    
    log("k_dev_writeMem_checkIncorrect_processIdInvalid end\n\0");
}

#[test_case]
fn k_dev_writeMem_checkIncorrect_deviceHandleInvalid() {
    log("k_dev_writeMem_checkIncorrect_deviceHandleInvalid start\n\0");

    let id: size_t = 0;
    let mut proc_id: procId_t = 0;
    install_dummy_writable_device(&id, &mut proc_id);
    let mut buffer: u8 = 0;
    let input_range = range_addr_t {
        offset: 0,
        sizeBytes: 1,
    };
    
    unsafe {
        devices[id as usize].holder = NONE_PROC_ID;
        assert_eq!(k_dev_writeMem(0, id, &mut buffer, input_range), gnwDeviceError::GDE_HANDLE_INVALID);
    }
    
    log("k_dev_writeMem_checkIncorrect_deviceHandleInvalid end\n\0");
}

#[test_case]
fn k_dev_writeMem_checkIncorrect_deviceNotStarted() {
    log("k_dev_writeMem_checkIncorrect_deviceNotStarted start\n\0");

    let id: size_t = 0;
    let mut proc_id: procId_t = 0;
    install_dummy_writable_device(&id, &mut proc_id);
    let mut buffer: u8 = 0;
    let input_range = range_addr_t {
        offset: 0,
        sizeBytes: 1,
    };
    
    unsafe {
        devices[id as usize].started = false;
        assert_eq!(k_dev_writeMem(0, id, &mut buffer, input_range), gnwDeviceError::GDE_INVALID_DEVICE_STATE);
    }
    
    log("k_dev_writeMem_checkIncorrect_deviceNotStarted end\n\0");
}

#[test_case]
fn k_dev_writeMem_checkIncorrect_deviceInputSizeBytesNull() {
    log("k_dev_writeMem_checkIncorrect_deviceInputSizeBytesNull start\n\0");

    let id: size_t = 0;
    let mut proc_id: procId_t = 0;
    install_dummy_writable_device(&id, &mut proc_id);
    let mut buffer: u8 = 0;
    let input_range = range_addr_t {
        offset: 0,
        sizeBytes: 1,
    };
    
    unsafe {
        devices[id as usize].desc.api.mem.desc.maxInputSizeBytes = 0;
        assert_eq!(k_dev_writeMem(proc_id, id, &mut buffer, input_range), gnwDeviceError::GDE_INVALID_OPERATION);
    }
    
    log("k_dev_writeMem_checkIncorrect_deviceInputSizeBytesNull end\n\0");
}

#[test_case]
fn k_dev_writeMem_checkIncorrect_inputRangeExceeded() {
    log("k_dev_writeMem_checkIncorrect_inputRangeExceeded start\n\0");

    let id: size_t = 0;
    let mut proc_id: procId_t = 0;
    install_dummy_writable_device(&id, &mut proc_id);
    let mut buffer: u8 = 0;
    let empty_input_range = range_addr_t {
        offset: 0,
        sizeBytes: 0,
    };
    let offset_empty_input_range = range_addr_t {
        offset: 1,
        sizeBytes: 0,
    };
    let offset_outside_empty_input_range = range_addr_t {
        offset: 2,
        sizeBytes: 0,
    };
    let offset_input_range = range_addr_t {
        offset: 1,
        sizeBytes: 1,
    };
    let offset_outside_input_range = range_addr_t {
        offset: 2,
        sizeBytes: 1,
    };
    let input_range_too_large = range_addr_t {
        offset: 0,
        sizeBytes: 2,
    };
    let input_range_too_large_2 = range_addr_t {
        offset: 0,
        sizeBytes: 2,
    };
    let offset_input_range_too_large = range_addr_t {
        offset: 1,
        sizeBytes: 2,
    };
    let offset_outside_input_range_too_large = range_addr_t {
        offset: 2,
        sizeBytes: 2,
    };
    
    unsafe {
        assert_eq!(k_dev_writeMem(0, id, &mut buffer, empty_input_range), gnwDeviceError::GDE_INVALID_PARAMETER);
        assert_eq!(k_dev_writeMem(0, id, &mut buffer, offset_empty_input_range), gnwDeviceError::GDE_INVALID_PARAMETER);
        assert_eq!(k_dev_writeMem(0, id, &mut buffer, offset_outside_empty_input_range), gnwDeviceError::GDE_INVALID_PARAMETER);
        assert_eq!(k_dev_writeMem(0, id, &mut buffer, offset_input_range), gnwDeviceError::GDE_INVALID_PARAMETER);
        assert_eq!(k_dev_writeMem(0, id, &mut buffer, offset_outside_input_range), gnwDeviceError::GDE_INVALID_PARAMETER);
        assert_eq!(k_dev_writeMem(0, id, &mut buffer, input_range_too_large), gnwDeviceError::GDE_INVALID_PARAMETER);
        assert_eq!(k_dev_writeMem(0, id, &mut buffer, input_range_too_large_2), gnwDeviceError::GDE_INVALID_PARAMETER);
        assert_eq!(k_dev_writeMem(0, id, &mut buffer, offset_input_range_too_large), gnwDeviceError::GDE_INVALID_PARAMETER);
        assert_eq!(k_dev_writeMem(0, id, &mut buffer, offset_outside_input_range_too_large), gnwDeviceError::GDE_INVALID_PARAMETER);
    }
    
    log("k_dev_writeMem_checkIncorrect_inputRangeExceeded end\n\0");
}

#[test_case]
fn k_dev_writeMem_checkIncorrect_writeNotSupported() {
    log("k_dev_writeMem_checkIncorrect_writeNotSupported start\n\0");

    let id: size_t = 0;
    let mut proc_id: procId_t = 0;
    install_dummy_writable_device(&id, &mut proc_id);
    let mut buffer: u8 = 0;
    let input_range = range_addr_t {
        offset: 0,
        sizeBytes: 1,
    };
    
    unsafe {
        devices[id as usize].desc.api.mem.routine.write = None;
        assert_eq!(k_dev_writeMem(proc_id, id, &mut buffer, input_range), gnwDeviceError::GDE_INVALID_OPERATION);
    }
    
    log("k_dev_writeMem_checkIncorrect_writeNotSupported end\n\0");
}

/*
    enum gnwDeviceError k_dev_writeChar(const procId_t processId, 
                                        const size_t deviceId,
                                        const char character)
*/

#[test_case]
fn k_dev_writeChar_checkCorrect() {
    log("k_dev_writeChar_checkCorrect start\n\0");

    let id: size_t = 0;
    let mut proc_id: procId_t = 0;
    install_dummy_writable_device(&id, &mut proc_id);

    unsafe {
        assert_eq!(k_dev_writeChar(proc_id, id, 0), gnwDeviceError::GDE_NONE);
        assert_eq!(DEV_CHAR_WRITE_CALLED, true);
        DEV_CHAR_WRITE_CALLED = false;
    }
    
    log("k_dev_writeChar_checkCorrect end\n\0");
}

#[test_case]
fn k_dev_writeChar_checkIncorrect_deviceNotInstalled() {
    log("k_dev_writeChar_checkIncorrect_deviceNotInstalled start\n\0");
    
    let id: size_t = 0;
    let mut proc_id: procId_t = 0;
    install_dummy_writable_device(&id, &mut proc_id);

    unsafe {
        devicesCount = 0;
        assert_eq!(k_dev_writeChar(proc_id, id, 0), gnwDeviceError::GDE_UNKNOWN);
    }
    
    log("k_dev_writeChar_checkIncorrect_deviceNotInstalled end\n\0");
}

#[test_case]
fn k_dev_writeChar_checkIncorrect_deviceIdInvalid() {
    log("k_dev_writeChar_checkIncorrect_deviceIdInvalid start\n\0");
    
    let id: size_t = 0;
    let mut proc_id: procId_t = 0;
    install_dummy_writable_device(&id, &mut proc_id);
    assert_eq!(id, 0);
    assert_eq!(proc_id, 0);

    unsafe {
        assert_eq!(k_dev_writeChar(proc_id, 1, 0), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(k_dev_writeChar(proc_id, MAX_DEVICES - 1, 0), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(k_dev_writeChar(proc_id, MAX_DEVICES, 0), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(k_dev_writeChar(proc_id, MAX_DEVICES + 1, 0), gnwDeviceError::GDE_UNKNOWN);
    }
    
    log("k_dev_writeChar_checkIncorrect_deviceIdInvalid end\n\0");
}

#[test_case]
fn k_dev_writeChar_checkIncorrect_processIdInvalid() {
    log("k_dev_writeChar_checkIncorrect_processIdInvalid start\n\0");

    let id: size_t = 0;
    let mut proc_id: procId_t = 0;
    install_dummy_writable_device(&id, &mut proc_id);

    unsafe {
        assert_eq!(k_dev_writeChar(NONE_PROC_ID - 1, id, 0), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(k_dev_writeChar(NONE_PROC_ID, id, 0), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(k_dev_writeChar(KERNEL_PROC_ID, id, 0), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(k_dev_writeChar(1, id, 0), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(k_dev_writeChar(MAX_PROC - 1, id, 0), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(k_dev_writeChar(MAX_PROC, id, 0), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(k_dev_writeChar(MAX_PROC + 1, id, 0), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
    }
    
    log("k_dev_writeChar_checkIncorrect_processIdInvalid end\n\0");
}

#[test_case]
fn k_dev_writeChar_checkIncorrect_deviceHandleInvalid() {
    log("k_dev_writeChar_checkIncorrect_deviceHandleInvalid start\n\0");

    let id: size_t = 0;
    let mut proc_id: procId_t = 0;
    install_dummy_writable_device(&id, &mut proc_id);
    
    unsafe {
        devices[id as usize].holder = NONE_PROC_ID;
        assert_eq!(k_dev_writeChar(0, id, 0), gnwDeviceError::GDE_HANDLE_INVALID);
    }
    
    log("k_dev_writeChar_checkIncorrect_deviceHandleInvalid end\n\0");
}

#[test_case]
fn k_dev_writeChar_checkIncorrect_deviceNotStarted() {
    log("k_dev_writeChar_checkIncorrect_deviceNotStarted start\n\0");

    let id: size_t = 0;
    let mut proc_id: procId_t = 0;
    install_dummy_writable_device(&id, &mut proc_id);
    
    unsafe {
        devices[id as usize].started = false;
        assert_eq!(k_dev_writeChar(0, id, 0), gnwDeviceError::GDE_INVALID_DEVICE_STATE);
    }
    
    log("k_dev_writeChar_checkIncorrect_deviceNotStarted end\n\0");
}

#[test_case]
fn k_dev_writeChar_checkIncorrect_isReadyNotSupported() {
    log("k_dev_writeChar_checkIncorrect_isReadyNotSupported start\n\0");

    let id: size_t = 0;
    let mut proc_id: procId_t = 0;
    install_dummy_writable_device(&id, &mut proc_id);
    
    unsafe {
        devices[id as usize].desc.api.charOut.routine.isReadyToWrite = None;
        assert_eq!(k_dev_writeChar(proc_id, id, 0), gnwDeviceError::GDE_INVALID_OPERATION);
    }
    
    log("k_dev_writeChar_checkIncorrect_isReadyNotSupported end\n\0");
}

#[test_case]
fn k_dev_writeChar_checkIncorrect_deviceNotReady() {
    log("k_dev_writeChar_checkIncorrect_deviceNotReady start\n\0");

    let id: size_t = 0;
    let mut proc_id: procId_t = 0;
    install_dummy_writable_device(&id, &mut proc_id);
    extern "C" fn char_out_is_ready_to_write() -> bool { return false; }
    unsafe {
        devices[id as usize].desc.api.charOut.routine.isReadyToWrite = Some(char_out_is_ready_to_write);
        assert_eq!(k_dev_writeChar(proc_id, id, 0), gnwDeviceError::GDE_INVALID_DEVICE_STATE);
    }
    
    log("k_dev_writeChar_checkIncorrect_deviceNotReady end\n\0");
}

#[test_case]
fn k_dev_writeChar_checkIncorrect_writeNotSupported() {
    log("k_dev_writeChar_checkIncorrect_writeNotSupported start\n\0");

    let id: size_t = 0;
    let mut proc_id: procId_t = 0;
    install_dummy_writable_device(&id, &mut proc_id);
    
    unsafe {
        devices[id as usize].desc.api.charOut.routine.write = None;
        assert_eq!(k_dev_writeChar(proc_id, id, 0), gnwDeviceError::GDE_INVALID_OPERATION);
    }
    
    log("k_dev_writeChar_checkIncorrect_writeNotSupported end\n\0");
}

#[test_case]
fn k_dev_writeChar_checkIncorrect_writeFailed() {
    log("k_dev_writeChar_checkIncorrect_writeFailed start\n\0");

    let id: size_t = 0;
    let mut proc_id: procId_t = 0;
    install_dummy_writable_device(&id, &mut proc_id);
    extern "C" fn char_out_write(_: i8) -> bool { return false; }
    unsafe {
        devices[id as usize].desc.api.charOut.routine.write = Some(char_out_write);
        assert_eq!(k_dev_writeChar(proc_id, id, 0), gnwDeviceError::GDE_OPERATION_FAILED);
    }
    
    log("k_dev_writeChar_checkIncorrect_writeFailed end\n\0");
}

/*
    enum gnwDeviceError k_dev_listen(const procId_t processId, 
                                     const size_t deviceId, 
                                     const gnwDeviceEventListener listener,
                                     const gnwDeviceEventDecoder decoder)
*/

#[test_case]
fn k_dev_listen_checkCorrect() {
    log("k_dev_listen_checkCorrect start\n\0");

    let proc_id: procId_t = install_dummy_process();
    assert_eq!(proc_id, 0);
    let device_id: size_t = 0;
    install_dummy_device(&device_id, false);
    assert_eq!(device_id, 0);
    install_dummy_device_holder(device_id, proc_id);
    extern "cdecl" fn listener(_unused: *const gnwDeviceEvent) {}
    extern "C" fn decoder(_: ptr_t, _: *const gnwDeviceEvent) {}

    unsafe {
        assert_eq!(k_dev_listen(proc_id, device_id, Some(listener), Some(decoder)), gnwDeviceError::GDE_NONE);
        assert!(devices[device_id as usize].listener == Some(listener));
        assert!(devices[device_id as usize].decoder == Some(decoder));
    }
    
    log("k_dev_listen_checkCorrect end\n\0");
}

#[test_case]
fn k_dev_listen_checkIncorrect_listenerNull() {
    log("k_dev_listen_checkIncorrect_listenerNull start\n\0");

    let proc_id: procId_t = install_dummy_process();
    assert_eq!(proc_id, 0);
    let device_id: size_t = 0;
    install_dummy_device(&device_id, false);
    assert_eq!(device_id, 0);
    install_dummy_device_holder(device_id, proc_id);
    extern "C" fn decoder(_: ptr_t, _: *const gnwDeviceEvent) {}

    unsafe {
        assert_eq!(k_dev_listen(proc_id, device_id, None, Some(decoder)), gnwDeviceError::GDE_LISTENER_INVALID);
    }
    
    log("k_dev_listen_checkIncorrect_listenerNull end\n\0");
}

#[test_case]
fn k_dev_listen_checkIncorrect_decoderNull() {
    log("k_dev_listen_checkIncorrect_decoderNull start\n\0");

    let proc_id: procId_t = install_dummy_process();
    assert_eq!(proc_id, 0);
    let device_id: size_t = 0;
    install_dummy_device(&device_id, false);
    assert_eq!(device_id, 0);
    install_dummy_device_holder(device_id, proc_id);
    extern "cdecl" fn listener(_unused: *const gnwDeviceEvent) {}

    unsafe {
        assert_eq!(k_dev_listen(proc_id, device_id, Some(listener), None), gnwDeviceError::GDE_DECODER_INVALID);
    }
    
    log("k_dev_listen_checkIncorrect_decoderNull end\n\0");
}

#[test_case]
fn k_dev_listen_checkIncorrect_deviceNotInstalled() {
    log("k_dev_listen_checkIncorrect_deviceNotInstalled start\n\0");

    let proc_id: procId_t = install_dummy_process();
    assert_eq!(proc_id, 0);
    let device_id: size_t = 0;
    install_dummy_device_holder(device_id, proc_id);
    extern "cdecl" fn listener(_unused: *const gnwDeviceEvent) {}
    extern "C" fn decoder(_: ptr_t, _: *const gnwDeviceEvent) {}

    unsafe {
        assert_eq!(k_dev_listen(proc_id, device_id, Some(listener), Some(decoder)), gnwDeviceError::GDE_UNKNOWN);
    }
    
    log("k_dev_listen_checkIncorrect_deviceNotInstalled end\n\0");
}

#[test_case]
fn k_dev_listen_checkIncorrect_deviceIdInvalid() {
    log("k_dev_listen_checkIncorrect_deviceIdInvalid start\n\0");

    let proc_id: procId_t = install_dummy_process();
    assert_eq!(proc_id, 0);
    let device_id: size_t = 0;
    install_dummy_device(&device_id, false);
    assert_eq!(device_id, 0);
    install_dummy_device_holder(device_id, proc_id);
    extern "cdecl" fn listener(_unused: *const gnwDeviceEvent) {}
    extern "C" fn decoder(_: ptr_t, _: *const gnwDeviceEvent) {}

    unsafe {
        assert_eq!(k_dev_listen(proc_id, 1, Some(listener), Some(decoder)), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(k_dev_listen(proc_id, MAX_DEVICES - 1, Some(listener), Some(decoder)), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(k_dev_listen(proc_id, MAX_DEVICES, Some(listener), Some(decoder)), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(k_dev_listen(proc_id, MAX_DEVICES + 1, Some(listener), Some(decoder)), gnwDeviceError::GDE_UNKNOWN);
    }
    
    log("k_dev_listen_checkIncorrect_deviceIdInvalid end\n\0");
}

#[test_case]
fn k_dev_listen_checkIncorrect_processIdInvalid() {
    log("k_dev_listen_checkIncorrect_processIdInvalid start\n\0");

    let proc_id: procId_t = install_dummy_process();
    assert_eq!(proc_id, 0);
    let device_id: size_t = 0;
    install_dummy_device(&device_id, false);
    assert_eq!(device_id, 0);
    install_dummy_device_holder(device_id, proc_id);
    extern "cdecl" fn listener(_unused: *const gnwDeviceEvent) {}
    extern "C" fn decoder(_: ptr_t, _: *const gnwDeviceEvent) {}

    unsafe {
        assert_eq!(k_dev_listen(NONE_PROC_ID - 1, device_id, Some(listener), Some(decoder)), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(k_dev_listen(NONE_PROC_ID, device_id, Some(listener), Some(decoder)), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(k_dev_listen(KERNEL_PROC_ID, device_id, Some(listener), Some(decoder)), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(k_dev_listen(1, device_id, Some(listener), Some(decoder)), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(k_dev_listen(MAX_PROC - 1, device_id, Some(listener), Some(decoder)), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(k_dev_listen(MAX_PROC, device_id, Some(listener), Some(decoder)), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(k_dev_listen(MAX_PROC + 1, device_id, Some(listener), Some(decoder)), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
    }
    
    log("k_dev_listen_checkIncorrect_processIdInvalid end\n\0");
}

#[test_case]
fn k_dev_listen_checkIncorrect_deviceHandleInvalid() {
    log("k_dev_listen_checkIncorrect_deviceHandleInvalid start\n\0");

    let proc_id: procId_t = install_dummy_process();
    assert_eq!(proc_id, 0);
    let device_id: size_t = 0;
    install_dummy_device(&device_id, false);
    assert_eq!(device_id, 0);
    install_dummy_device_holder(device_id, proc_id);
    extern "cdecl" fn listener(_unused: *const gnwDeviceEvent) {}
    extern "C" fn decoder(_: ptr_t, _: *const gnwDeviceEvent) {}

    unsafe {
        devices[device_id as usize].holder = NONE_PROC_ID;
        assert_eq!(k_dev_listen(proc_id, device_id, Some(listener), Some(decoder)), gnwDeviceError::GDE_HANDLE_INVALID);
    }
    
    log("k_dev_listen_checkIncorrect_deviceHandleInvalid end\n\0");
}

#[test_case]
fn k_dev_listen_checkIncorrect_deviceNotStarted() {
    log("k_dev_listen_checkIncorrect_deviceNotStarted start\n\0");

    let proc_id: procId_t = install_dummy_process();
    assert_eq!(proc_id, 0);
    let device_id: size_t = 0;
    install_dummy_device(&device_id, false);
    assert_eq!(device_id, 0);
    install_dummy_device_holder(device_id, proc_id);
    extern "cdecl" fn listener(_unused: *const gnwDeviceEvent) {}
    extern "C" fn decoder(_: ptr_t, _: *const gnwDeviceEvent) {}

    unsafe {
        devices[device_id as usize].started = false;
        assert_eq!(k_dev_listen(proc_id, device_id, Some(listener), Some(decoder)), gnwDeviceError::GDE_INVALID_DEVICE_STATE);
    }
    
    log("k_dev_listen_checkIncorrect_deviceNotStarted end\n\0");
}

#[test_case]
fn k_dev_listen_checkIncorrect_listenerAlreadySet() {
    log("k_dev_listen_checkIncorrect_listenerAlreadySet start\n\0");

    let proc_id: procId_t = install_dummy_process();
    assert_eq!(proc_id, 0);
    let device_id: size_t = 0;
    install_dummy_device(&device_id, false);
    assert_eq!(device_id, 0);
    install_dummy_device_holder(device_id, proc_id);
    extern "cdecl" fn listener(_unused: *const gnwDeviceEvent) {}
    extern "C" fn decoder(_: ptr_t, _: *const gnwDeviceEvent) {}

    unsafe {
        devices[device_id as usize].listener = Some(listener);
        assert_eq!(k_dev_listen(proc_id, device_id, Some(listener), Some(decoder)), gnwDeviceError::GDE_ALREADY_SET);
    }
    
    log("k_dev_listen_checkIncorrect_listenerAlreadySet end\n\0");
}

/*
    enum gnwDeviceError k_dev_getParam(const size_t deviceId,
                                       const struct gnwDeviceParamDescriptor paramDescriptor,
                                       size_t * const absResult)
*/

#[test_case]
fn k_dev_getParam_checkCorrect() {
    log("k_dev_getParam_checkCorrect start\n\0");

    let device_id: size_t = 0;
    install_dummy_device(&device_id, true);
    assert_eq!(device_id, 0);
    let abs_result: size_t = 0;
    let param_descriptor = gnwDeviceParamDescriptor {
        param: 0,
        subParam: 0,
        paramIndex: 0,
    };

    unsafe {
        assert_eq!(k_dev_getParam(device_id, param_descriptor, &abs_result), gnwDeviceError::GDE_NONE);
        assert_eq!(DEV_GET_PARAM_CALLED, true);
        DEV_GET_PARAM_CALLED = false;
    }
    
    log("k_dev_getParam_checkCorrect end\n\0");
}

#[test_case]
fn k_dev_getParam_checkIncorrect_absResultNull() {
    log("k_dev_getParam_checkIncorrect_absResultNull start\n\0");

    let device_id: size_t = 0;
    install_dummy_device(&device_id, true);
    assert_eq!(device_id, 0);
    let param_descriptor = gnwDeviceParamDescriptor {
        param: 0,
        subParam: 0,
        paramIndex: 0,
    };

    unsafe {
        assert_eq!(k_dev_getParam(device_id, param_descriptor, null()), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
    }
    
    log("k_dev_getParam_checkIncorrect_absResultNull end\n\0");
}

#[test_case]
fn k_dev_getParam_checkIncorrect_deviceNotInstalled() {
    log("k_dev_getParam_checkIncorrect_deviceNotInstalled start\n\0");

    let abs_result: size_t = 0;
    let param_descriptor = gnwDeviceParamDescriptor {
        param: 0,
        subParam: 0,
        paramIndex: 0,
    };

    unsafe {
        assert_eq!(k_dev_getParam(0, param_descriptor, &abs_result), gnwDeviceError::GDE_ID_INVALID);
    }
    
    log("k_dev_getParam_checkIncorrect_deviceNotInstalled end\n\0");
}

#[test_case]
fn k_dev_getParam_checkIncorrect_deviceIdInvalid() {
    log("k_dev_getParam_checkIncorrect_deviceIdInvalid start\n\0");

    let device_id: size_t = 0;
    install_dummy_device(&device_id, true);
    assert_eq!(device_id, 0);
    let abs_result: size_t = 0;
    let param_descriptor = gnwDeviceParamDescriptor {
        param: 0,
        subParam: 0,
        paramIndex: 0,
    };

    unsafe {
        assert_eq!(k_dev_getParam(1, param_descriptor, &abs_result), gnwDeviceError::GDE_ID_INVALID);
        assert_eq!(k_dev_getParam(MAX_DEVICES - 1, param_descriptor, &abs_result), gnwDeviceError::GDE_ID_INVALID);
        assert_eq!(k_dev_getParam(MAX_DEVICES, param_descriptor, &abs_result), gnwDeviceError::GDE_ID_INVALID);
        assert_eq!(k_dev_getParam(MAX_DEVICES + 1, param_descriptor, &abs_result), gnwDeviceError::GDE_ID_INVALID);
    }
    
    log("k_dev_getParam_checkIncorrect_deviceIdInvalid end\n\0");
}

#[test_case]
fn k_dev_getParam_checkIncorrect_getParamNotSupported() {
    log("k_dev_getParam_checkIncorrect_getParamNotSupported start\n\0");

    let device_id: size_t = 0;
    install_dummy_device(&device_id, true);
    assert_eq!(device_id, 0);
    let abs_result: size_t = 0;
    let param_descriptor = gnwDeviceParamDescriptor {
        param: 0,
        subParam: 0,
        paramIndex: 0,
    };

    unsafe {
        devices[device_id as usize].desc.api.system.routine.getParam = None;
        assert_eq!(k_dev_getParam(device_id, param_descriptor, &abs_result), gnwDeviceError::GDE_INVALID_OPERATION);
    }
    
    log("k_dev_getParam_checkIncorrect_getParamNotSupported end\n\0");
}

#[test_case]
fn k_dev_getParam_checkIncorrect_getParamReturnsFalse() {
    log("k_dev_getParam_checkIncorrect_getParamReturnsFalse start\n\0");

    let device_id: size_t = 0;
    install_dummy_device(&device_id, true);
    assert_eq!(device_id, 0);
    let abs_result: size_t = 0;
    let param_descriptor = gnwDeviceParamDescriptor {
        param: 0,
        subParam: 0,
        paramIndex: 0,
    };
    extern "C" fn get_param(_: u32, _: u32, _: u32, _: *mut u32) -> bool { return false; }

    unsafe {
        devices[device_id as usize].desc.api.system.routine.getParam = Some(get_param);
        assert_eq!(k_dev_getParam(device_id, param_descriptor, &abs_result), gnwDeviceError::GDE_OPERATION_FAILED);
    }
    
    log("k_dev_getParam_checkIncorrect_getParamReturnsFalse end\n\0");
}

/*
    enum gnwDeviceError k_dev_setParam(const procId_t procId,
                                       const size_t deviceId,
                                       const struct gnwDeviceParamDescriptor paramDescriptor,
                                       const size_t value)
*/

#[test_case]
fn k_dev_setParam_checkCorrect() {
    log("k_dev_setParam_checkCorrect start\n\0");

    let device_id: size_t = 0;
    install_dummy_device(&device_id, true);
    let proc_id: procId_t = install_dummy_process();
    assert_eq!(device_id, 0);
    assert_eq!(proc_id, 0);
    install_dummy_device_holder(device_id, proc_id);
    let param_descriptor = gnwDeviceParamDescriptor {
        param: 0,
        subParam: 0,
        paramIndex: 0,
    };

    unsafe {
        assert_eq!(k_dev_setParam(proc_id, device_id, param_descriptor, 0), gnwDeviceError::GDE_NONE);
        assert_eq!(DEV_SET_PARAM_CALLED, true);
        DEV_SET_PARAM_CALLED = false;
    }
    
    log("k_dev_setParam_checkCorrect end\n\0");
}

#[test_case]
fn k_dev_setParam_checkIncorrect_deviceNotInstalled() {
    log("k_dev_setParam_checkIncorrect_deviceNotInstalled start\n\0");

    let device_id: size_t = 0;
    let proc_id: procId_t = install_dummy_process();
    assert_eq!(proc_id, 0);
    let param_descriptor = gnwDeviceParamDescriptor {
        param: 0,
        subParam: 0,
        paramIndex: 0,
    };

    unsafe {
        assert_eq!(k_dev_setParam(proc_id, device_id, param_descriptor, 0), gnwDeviceError::GDE_UNKNOWN);
    }
    
    log("k_dev_setParam_checkIncorrect_deviceNotInstalled end\n\0");
}

#[test_case]
fn k_dev_setParam_checkIncorrect_deviceIdInvalid() {
    log("k_dev_setParam_checkIncorrect_deviceIdInvalid start\n\0");

    let device_id: size_t = 0;
    install_dummy_device(&device_id, true);
    let proc_id: procId_t = install_dummy_process();
    assert_eq!(device_id, 0);
    assert_eq!(proc_id, 0);
    install_dummy_device_holder(device_id, proc_id);
    let param_descriptor = gnwDeviceParamDescriptor {
        param: 0,
        subParam: 0,
        paramIndex: 0,
    };

    unsafe {
        assert_eq!(k_dev_setParam(proc_id, 1, param_descriptor, 0), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(k_dev_setParam(proc_id, MAX_DEVICES - 1, param_descriptor, 0), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(k_dev_setParam(proc_id, MAX_DEVICES, param_descriptor, 0), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(k_dev_setParam(proc_id, MAX_DEVICES + 1, param_descriptor, 0), gnwDeviceError::GDE_UNKNOWN);
    }
    
    log("k_dev_setParam_checkIncorrect_deviceIdInvalid end\n\0");
}

#[test_case]
fn k_dev_setParam_checkIncorrect_processIdInvalid() {
    log("k_dev_setParam_checkIncorrect_processIdInvalid start\n\0");

    let device_id: size_t = 0;
    install_dummy_device(&device_id, true);
    let proc_id: procId_t = install_dummy_process();
    assert_eq!(device_id, 0);
    assert_eq!(proc_id, 0);
    install_dummy_device_holder(device_id, proc_id);
    let param_descriptor = gnwDeviceParamDescriptor {
        param: 0,
        subParam: 0,
        paramIndex: 0,
    };

    unsafe {
        assert_eq!(k_dev_setParam(NONE_PROC_ID - 1, device_id, param_descriptor, 0), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(k_dev_setParam(NONE_PROC_ID, device_id, param_descriptor, 0), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(k_dev_setParam(KERNEL_PROC_ID, device_id, param_descriptor, 0), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(k_dev_setParam(1, device_id, param_descriptor, 0), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(k_dev_setParam(MAX_PROC - 1, device_id, param_descriptor, 0), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(k_dev_setParam(MAX_PROC, device_id, param_descriptor, 0), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        assert_eq!(k_dev_setParam(MAX_PROC + 1, device_id, param_descriptor, 0), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
    }
    
    log("k_dev_setParam_checkIncorrect_processIdInvalid end\n\0");
}

#[test_case]
fn k_dev_setParam_checkIncorrect_deviceHandleInvalid() {
    log("k_dev_setParam_checkIncorrect_deviceHandleInvalid start\n\0");

    let device_id: size_t = 0;
    install_dummy_device(&device_id, true);
    let proc_id: procId_t = install_dummy_process();
    assert_eq!(device_id, 0);
    assert_eq!(proc_id, 0);
    install_dummy_device_holder(device_id, proc_id);
    let param_descriptor = gnwDeviceParamDescriptor {
        param: 0,
        subParam: 0,
        paramIndex: 0,
    };

    unsafe {
        devices[device_id as usize].holder = NONE_PROC_ID;
        assert_eq!(k_dev_setParam(proc_id, device_id, param_descriptor, 0), gnwDeviceError::GDE_HANDLE_INVALID);
    }
    
    log("k_dev_setParam_checkIncorrect_deviceHandleInvalid end\n\0");
}

#[test_case]
fn k_dev_setParam_checkIncorrect_deviceNotStarted() {
    log("k_dev_setParam_checkIncorrect_deviceNotStarted start\n\0");

    let device_id: size_t = 0;
    install_dummy_device(&device_id, true);
    let proc_id: procId_t = install_dummy_process();
    assert_eq!(device_id, 0);
    assert_eq!(proc_id, 0);
    install_dummy_device_holder(device_id, proc_id);
    let param_descriptor = gnwDeviceParamDescriptor {
        param: 0,
        subParam: 0,
        paramIndex: 0,
    };

    unsafe {
        devices[device_id as usize].started = false;
        assert_eq!(k_dev_setParam(proc_id, device_id, param_descriptor, 0), gnwDeviceError::GDE_INVALID_DEVICE_STATE);
    }
    
    log("k_dev_setParam_checkIncorrect_deviceNotStarted end\n\0");
}

#[test_case]
fn k_dev_setParam_checkIncorrect_getParamNotSupported() {
    log("k_dev_setParam_checkIncorrect_getParamNotSupported start\n\0");

    let device_id: size_t = 0;
    install_dummy_device(&device_id, true);
    let proc_id: procId_t = install_dummy_process();
    assert_eq!(device_id, 0);
    assert_eq!(proc_id, 0);
    install_dummy_device_holder(device_id, proc_id);
    let param_descriptor = gnwDeviceParamDescriptor {
        param: 0,
        subParam: 0,
        paramIndex: 0,
    };

    unsafe {
        devices[device_id as usize].desc.api.system.routine.setParam = None;
        assert_eq!(k_dev_setParam(proc_id, device_id, param_descriptor, 0), gnwDeviceError::GDE_INVALID_OPERATION);
    }
    
    log("k_dev_setParam_checkIncorrect_getParamNotSupported end\n\0");
}

#[test_case]
fn k_dev_setParam_checkIncorrect_getParamReturnsFalse() {
    log("k_dev_setParam_checkIncorrect_getParamReturnsFalse start\n\0");

    let device_id: size_t = 0;
    install_dummy_device(&device_id, true);
    let proc_id: procId_t = install_dummy_process();
    assert_eq!(device_id, 0);
    assert_eq!(proc_id, 0);
    install_dummy_device_holder(device_id, proc_id);
    let param_descriptor = gnwDeviceParamDescriptor {
        param: 0,
        subParam: 0,
        paramIndex: 0,
    };
    extern "C" fn set_param(_: u32, _: u32, _: u32, _: u32) -> bool { return false; }

    unsafe {
        devices[device_id as usize].desc.api.system.routine.setParam = Some(set_param);
        assert_eq!(k_dev_setParam(proc_id, device_id, param_descriptor, 0), gnwDeviceError::GDE_OPERATION_FAILED);
    }
    
    log("k_dev_setParam_checkIncorrect_getParamReturnsFalse end\n\0");
}

/*
    PRIVATE enum gnwDeviceError validateEmitter(const size_t * const devIdPtr)
*/

#[test_case]
fn validateEmitter_checkCorrect() {
    log("validateEmitter_checkCorrect start\n\0");

    let device_id: size_t = 0;
    install_dummy_device(&device_id, true);
    assert_eq!(device_id, 0);
    
    unsafe {
        assert_eq!(validateEmitter(&device_id), gnwDeviceError::GDE_NONE);
        assert_eq!(device_id, 0);
    }
    
    log("validateEmitter_checkCorrect end\n\0");
}

#[test_case]
fn validateEmitter_checkIncorrect_devIdPtrNull() {
    log("validateEmitter_checkIncorrect_devIdPtrNull start\n\0");

    let device_id: size_t = 0;
    install_dummy_device(&device_id, true);
    assert_eq!(device_id, 0);
    
    unsafe {
        assert_eq!(validateEmitter(null()), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
    }
    
    log("validateEmitter_checkIncorrect_devIdPtrNull end\n\0");
}

#[test_case]
fn validateEmitter_checkIncorrect_deviceIdInvalid() {
    log("validateEmitter_checkIncorrect_deviceIdInvalid start\n\0");

    let mut 
    device_id: size_t = 0;
    install_dummy_device(&device_id, true);
    assert_eq!(device_id, 0);

    unsafe {
        device_id = 1;
        assert_eq!(validateEmitter(&device_id), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(device_id, 1);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        device_id = MAX_DEVICES - 1;
        assert_eq!(validateEmitter(&device_id), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(device_id, MAX_DEVICES - 1);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        device_id = MAX_DEVICES;
        assert_eq!(validateEmitter(&device_id), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(device_id, MAX_DEVICES);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
        device_id = MAX_DEVICES + 1;
        assert_eq!(validateEmitter(&device_id), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(device_id, MAX_DEVICES + 1);
        assert_eq!(KERNEL_PANIC_FLAG, true);
    }
    
    log("validateEmitter_checkIncorrect_deviceIdInvalid end\n\0");
}

#[test_case]
fn validateEmitter_checkIncorrect_deviceNotStarted() {
    log("validateEmitter_checkIncorrect_deviceNotStarted start\n\0");

    let device_id: size_t = 0;
    install_dummy_device(&device_id, true);
    assert_eq!(device_id, 0);
    
    unsafe {
        devices[device_id as usize].started = false;
        assert_eq!(validateEmitter(&device_id), gnwDeviceError::GDE_INVALID_DEVICE_STATE);
        assert_eq!(device_id, 0);
    }
    
    log("validateEmitter_checkIncorrect_deviceNotStarted end\n\0");
}

/*
    PRIVATE enum gnwDeviceError validateListenerInvocation(const size_t deviceId)
*/

#[test_case]
fn validateListenerInvocation_checkCorrect() {
    log("validateListenerInvocation_checkCorrect start\n\0");

    let device_id: size_t = 0;
    install_dummy_device(&device_id, true);
    let proc_id: procId_t = install_dummy_process();
    assert_eq!(device_id, 0);
    assert_eq!(proc_id, 0);
    install_dummy_device_listener(device_id, proc_id);
    
    unsafe {
        assert_eq!(validateListenerInvocation(device_id), gnwDeviceError::GDE_NONE);
    }
    
    log("validateListenerInvocation_checkCorrect end\n\0");
}

#[test_case]
fn validateListenerInvocation_checkIncorrect_listenerNull() {
    log("validateListenerInvocation_checkIncorrect_listenerNull start\n\0");

    let device_id: size_t = 0;
    install_dummy_device(&device_id, true);
    let proc_id: procId_t = install_dummy_process();
    assert_eq!(device_id, 0);
    assert_eq!(proc_id, 0);
    install_dummy_device_listener(device_id, proc_id);
    
    unsafe {
        devices[device_id as usize].listener = None;
        assert_eq!(validateListenerInvocation(device_id), gnwDeviceError::GDE_NOT_FOUND);
    }
    
    log("validateListenerInvocation_checkIncorrect_listenerNull end\n\0");
}

#[test_case]
fn validateListenerInvocation_checkIncorrect_decoderNull() {
    log("validateListenerInvocation_checkIncorrect_decoderNull start\n\0");

    let device_id: size_t = 0;
    install_dummy_device(&device_id, true);
    let proc_id: procId_t = install_dummy_process();
    assert_eq!(device_id, 0);
    assert_eq!(proc_id, 0);
    install_dummy_device_listener(device_id, proc_id);
    
    unsafe {
        devices[device_id as usize].decoder = None;
        assert_eq!(validateListenerInvocation(device_id), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
    }
    
    log("validateListenerInvocation_checkIncorrect_decoderNull end\n\0");
}

#[test_case]
fn validateListenerInvocation_checkIncorrect_noHolderProcId() {
    log("validateListenerInvocation_checkIncorrect_noHolderProcId start\n\0");

    let device_id: size_t = 0;
    install_dummy_device(&device_id, true);
    let proc_id: procId_t = install_dummy_process();
    assert_eq!(device_id, 0);
    assert_eq!(proc_id, 0);
    install_dummy_device_listener(device_id, proc_id);
    
    unsafe {
        devices[device_id as usize].holder = NONE_PROC_ID;
        assert_eq!(validateListenerInvocation(device_id), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
        KERNEL_PANIC_FLAG = false;
    }
    
    log("validateListenerInvocation_checkIncorrect_noHolderProcId end\n\0");
}

/*
    enum gnwDeviceError k_dev_emit(const struct gnwDeviceEvent * const eventPtr)
*/

#[test_case]
fn k_dev_emit_checkCorrect() {
    log("k_dev_emit_checkCorrect start\n\0");

    let device_id: size_t = 0;
    let proc_id: procId_t = install_dummy_process();
    assert_eq!(proc_id, 0);
    install_dummy_device(&device_id, true);
    assert_eq!(device_id, 0);
    install_dummy_device_listener(device_id, proc_id);
    let mut data: u8 = 0;
    let event = gnwDeviceEvent {
        r#type: 69,
        data: &mut data,
        dataSizeBytes: 1,
    };
    
    unsafe {
        isrStackHeight = 1;
        pTab[0].lockMask = k_proc_lockType::PLT_EVENT as i32 |
                           k_proc_lockType::PLT_IPC as i32;
        k_hal_servicedDevIdPtr = &device_id;
        let index: u32 = 1;
        assert_eq!(KERNEL_PANIC_FLAG, false);
        assert_eq!(k_dev_emit(&event), gnwDeviceError::GDE_NONE);
        assert_eq!(data, 0);
        assert_eq!(rlp_main[proc_id as usize].endIndex, index);
        assert_eq!(rlp_main[proc_id as usize].finishedIndex, index - 1);
        assert_eq!(rlp_main[proc_id as usize].queue[index as usize].reserved, true);
        assert_eq!(rlp_main[proc_id as usize].queue[index as usize].handled, false);
        assert_eq!(rlp_main[proc_id as usize].queue[index as usize].dataHandled, false);
        assert_eq!(rlp_main[proc_id as usize].queue[index as usize].item.format, gnwEventFormat::GEF_PTR);
        log("1\n\0");
        assert_eq!(rlp_main[proc_id as usize].queue[index as usize].item.routine._handle, 1);
        log("2\n\0");
        assert_eq!(rlp_main[proc_id as usize].queue[index as usize].item.dataSizeBytes, 1);
        log("3\n\0");
        assert_eq!(rlp_main[proc_id as usize].queue[index as usize].item.decodedDataSizeBytes, 1);
        log("4\n\0");
        assert_eq!(rlp_main[proc_id as usize].queue[index as usize].item.decode, None);
        log("5\n\0");
        for byte_offset in 0..DISPATCH_MAX_DATA_SIZE_BYTES {
            log("6\n\0");
            assert_eq!(rlp_main[proc_id as usize].queue[index as usize].data[byte_offset as usize], 1);
        }
        log("7\n\0");
        assert_eq!(pTab[proc_id as usize].lockMask, k_proc_lockType::PLT_IPC as i32);
        log("8\n\0");
        assert_eq!(executionTimeCounter, GRANULARITY_MS);
    }
    
    log("k_dev_emit_checkCorrect end\n\0");
}

// enum gnwDeviceError k_dev_emit(const struct gnwDeviceEvent * const eventPtr) {
//     if (!eventPtr) {
//         OOPS("Nullptr", GDE_UNKNOWN);
//     }
//     enum gnwDeviceError err = validateEmitter(k_hal_servicedDevIdPtr);
//     if (err) {
//         return err;
//     }
//     err = validateListenerInvocation(*k_hal_servicedDevIdPtr);
//     if (err != GDE_NONE) {
//         return err;
//     }

//     struct device *dev = &devices[*k_hal_servicedDevIdPtr];
//     enum k_proc_error callbackErr = k_proc_callback_invoke_ptr(dev->holder, 
//                                                                (gnwEventListener_ptr)dev->listener,
//                                                                (ptr_t)eventPtr,
//                                                                eventPtr->dataSizeBytes + sizeof(struct gnwDeviceEvent),
//                                                                sizeof(struct gnwDeviceEvent),
//                                                                (gnwRunLoopDataEncodingRoutine)gnwDeviceEvent_encode,
//                                                                (gnwRunLoopDataEncodingRoutine)dev->decoder);
//     switch (callbackErr) {
//     case PE_NONE:
//         return GDE_NONE;
//     case PE_IGNORED:
//         return GDE_HANDLE_INVALID;
//     case PE_ACCESS_VIOLATION:
//         return GDE_LISTENER_INVALID;
//     default:
//         return GDE_UNKNOWN;
//     }
// }

// void k_dev_procCleanup(const procId_t procId) {
//     for (size_t devId = 0; devId < MAX_DEVICES; ++devId) {
//         if (devices[devId].holder == procId) {
//             CRITICAL_SECTION(
//                 k_dev_releaseHold(procId, devId);
//             )
//         }
//     }
// }


// #[macro_export]
// macro_rules! my_test_case {
//     ($function_name:ident, $test_name:ident, $body:block) => {
//         concat_idents!(test_name = $function_name, _, $test_name {
//             #[test_case]
//             fn test_name() {
//                 //   log("Start {}, $test_name);
//                 $body
//                 //   log("Finished {}, $test_name);
//             }
//         });            
//     }
// }

// my_test_case! { dick, checkCorrect, {
//     assert_eq!(0,0);
// }}

// my_test_case! { dick, checkIncorrect, {
//     assert_eq!(1,1);
// }}