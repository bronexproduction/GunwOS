pub mod helpers;

use utils::*;
use kernel_symbols::*;
use self::helpers::*;
use core::ptr::null;
use core::ptr::null_mut;
use core::slice::from_raw_parts;
use core::mem;

static PID_LIST: [procId_t; 8] = [NONE_PROC_ID - 1,
                                  NONE_PROC_ID,
                                  KERNEL_PROC_ID,
                                  0,
                                  1,
                                  MAX_PROC - 1,
                                  MAX_PROC,
                                  MAX_PROC + 1];
static INVALID_PID_LIST: [procId_t; 7] = [NONE_PROC_ID - 1,
                                          NONE_PROC_ID,
                                          KERNEL_PROC_ID,
                                          1,
                                          MAX_PROC - 1,
                                          MAX_PROC,
                                          MAX_PROC + 1];
static DEVICE_ID_LIST: [size_t; 5] = [0,
                                      1,
                                      MAX_DEVICES - 1,
                                      MAX_DEVICES,
                                      MAX_DEVICES + 1];
static INVALID_DEVICE_ID_LIST: [size_t; 4] = [1,
                                              MAX_DEVICES - 1,
                                              MAX_DEVICES,
                                              MAX_DEVICES + 1];

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

macro_rules! validateInstalledId_preconditions {
    () => {
        unsafe {
            devicesCount = MAX_DEVICES / 2;
        }
    };
}

#[test_case]
fn validateInstalledId_checkCorrect() {
    log("validateInstalledId_checkCorrect start\n\0");

    validateInstalledId_preconditions!();

    unsafe {
        assert_eq!(validateInstalledId(0), true);
        assert_eq!(validateInstalledId(devicesCount - 1), true);
    }

    log("validateInstalledId_checkCorrect end\n\0");
}

#[test_case]
fn validateInstalledId_checkIncorrect() {
    log("validateInstalledId_checkIncorrect start\n\0");

    validateInstalledId_preconditions!();

    unsafe {
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

macro_rules! validateStartedDevice_preconditions {
    () => {
        unsafe {
            devicesCount = 1;
            devices[0].holder = 0;
            devices[0].started = true;
            assert_eq!(install_dummy_process(), 0);
        }
    };
}

#[allow(non_snake_case)]
fn validateStartedDevice_expect(proc_id: procId_t, device_id: size_t, error: gnwDeviceError, kernel_panic: bool) {
    unsafe {
        assert_eq!(validateStartedDevice(proc_id, device_id), error);
        assert_eq!(KERNEL_PANIC_FLAG, kernel_panic);
        KERNEL_PANIC_FLAG = false;
    }
}

#[test_case]
fn validateStartedDevice_checkCorrect() {
    log("validateStartedDevice_checkCorrect start\n\0");

    validateStartedDevice_preconditions!();

    validateStartedDevice_expect(0, 0, gnwDeviceError::GDE_NONE, false);

    log("validateStartedDevice_checkCorrect end\n\0");
}

#[test_case]
fn validateStartedDevice_checkIncorrect_noHolderProcId() {
    log("validateStartedDevice_checkIncorrect_noHolderProcId start\n\0");

    validateStartedDevice_preconditions!();

    unsafe {
        devices[0].holder = NONE_PROC_ID;
    }

    validateStartedDevice_expect(NONE_PROC_ID - 1, 0, gnwDeviceError::GDE_UNKNOWN, true);
    validateStartedDevice_expect(NONE_PROC_ID, 0, gnwDeviceError::GDE_UNKNOWN, true);
    validateStartedDevice_expect(KERNEL_PROC_ID, 0, gnwDeviceError::GDE_UNKNOWN, true);
    validateStartedDevice_expect(1, 0, gnwDeviceError::GDE_UNKNOWN, true);
    validateStartedDevice_expect(MAX_PROC - 1, 0, gnwDeviceError::GDE_UNKNOWN, true);
    validateStartedDevice_expect(MAX_PROC, 0, gnwDeviceError::GDE_UNKNOWN, true);
    validateStartedDevice_expect(MAX_PROC + 1, 0, gnwDeviceError::GDE_UNKNOWN, true);

    log("validateStartedDevice_checkIncorrect_noHolderProcId end\n\0");
}

#[test_case]
fn validateStartedDevice_checkIncorrect_deviceIdInvalid() {
    log("validateStartedDevice_checkIncorrect_deviceIdInvalid start\n\0");

    validateStartedDevice_preconditions!();

    for did in INVALID_DEVICE_ID_LIST {
        validateStartedDevice_expect(0, did, gnwDeviceError::GDE_UNKNOWN, false);
    }

    log("validateStartedDevice_checkIncorrect_deviceIdInvalid end\n\0");
}

#[test_case]
fn validateStartedDevice_checkIncorrect_handleInvalid() {
    log("validateStartedDevice_checkIncorrect_handleInvalid start\n\0");

    validateStartedDevice_preconditions!();

    unsafe {
        devices[0].holder = NONE_PROC_ID;
    }

    validateStartedDevice_expect(0, 0, gnwDeviceError::GDE_HANDLE_INVALID, false);

    log("validateStartedDevice_checkIncorrect_handleInvalid end\n\0");
}

#[test_case]
fn validateStartedDevice_checkIncorrect_deviceNotStarted() {
    log("validateStartedDevice_checkIncorrect_deviceNotStarted start\n\0");

    validateStartedDevice_preconditions!();

    unsafe {
        devices[0].started = false;
    }

    validateStartedDevice_expect(0, 0, gnwDeviceError::GDE_INVALID_DEVICE_STATE, false);

    log("validateStartedDevice_checkIncorrect_deviceNotStarted end\n\0");
}

/*
    enum gnwDriverError k_dev_install(size_t * const id, const struct gnwDeviceDescriptor * const descriptor)
*/

macro_rules! k_dev_install_preconditions {
    ($device_id:ident, $device_desc:ident, $device_desc_creator:ident) => {
        let $device_id: size_t = 0;
        #[allow(unused_mut)] let mut $device_desc = $device_desc_creator();
    };
}

#[test_case]
fn k_dev_install_checkCorrect_simple() {
    log("k_dev_install_checkCorrect_simple start\n\0");

    k_dev_install_preconditions!(device_id, dev_desc, create_valid_device_desc_minimal);

    unsafe {
        assert_eq!(k_dev_install(&device_id, &dev_desc), gnwDriverError::GDRE_NONE);
    }

    log("k_dev_install_checkCorrect_simple end\n\0");
}

#[test_case]
fn k_dev_install_checkCorrect_complex() {
    log("k_dev_install_checkCorrect_complex start\n\0");

    k_dev_install_preconditions!(device_id, dev_desc, create_valid_device_desc_complex);

    /*
        Additional verification - installing the last device
    */
    unsafe {
        devicesCount = MAX_DEVICES - 1;
    }
    
    unsafe {
        /*
            Check install
        */
        assert_eq!(k_dev_install(&device_id, &dev_desc), gnwDriverError::GDRE_NONE);
        
        /*
            Check driver init called
        */
        assert_eq!(DRIVER_INIT_CALLED, true);
        
        /*
            Check device count
        */
        assert_eq!(devicesCount, MAX_DEVICES);
        
        /*
            Check devices array
        */
        assert_eq!(devices[device_id as usize].desc, dev_desc);
        assert_eq!(devices[device_id as usize].initialized, true);
        assert_eq!(devices[device_id as usize].started, false);
        assert_eq!(devices[device_id as usize].holder, NONE_PROC_ID);
        assert_eq!(devices[device_id as usize].listener, None);
        assert_eq!(devices[device_id as usize].decoder, None);
        
        /*
            Check device identifier
        */            
        assert_eq!(device_id, MAX_DEVICES - 1);
    }

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

    k_dev_install_preconditions!(device_id, _unused, create_empty_device_desc);

    unsafe {
        devicesCount = MAX_DEVICES;
        assert_eq!(k_dev_install(&device_id, core::ptr::null()), gnwDriverError::GDRE_LIMIT_REACHED);
        devicesCount = MAX_DEVICES + 1;
        assert_eq!(k_dev_install(&device_id, core::ptr::null()), gnwDriverError::GDRE_LIMIT_REACHED);
    }

    log("k_dev_install_checkIncorrect_deviceLimitReached end\n\0");
}

#[test_case]
fn k_dev_install_checkIncorrect_nullDescriptor() {
    log("k_dev_install_checkIncorrect_nullDescriptor start\n\0");

    k_dev_install_preconditions!(device_id, _unused, create_empty_device_desc);

    unsafe {
        assert_eq!(k_dev_install(&device_id, core::ptr::null()), gnwDriverError::GDRE_INVALID_DESCRIPTOR);
    }

    log("k_dev_install_checkIncorrect_nullDescriptor end\n\0");
}

#[test_case]
fn k_dev_install_checkIncorrect_invalidDescriptor() {
    log("k_dev_install_checkIncorrect_invalidDescriptor start\n\0");

    k_dev_install_preconditions!(device_id, dev_desc, create_empty_device_desc);

    unsafe {
        assert_eq!(k_dev_install(&device_id, &dev_desc), gnwDriverError::GDRE_INVALID_DESCRIPTOR);
    }

    log("k_dev_install_checkIncorrect_invalidDescriptor end\n\0");
}

#[test_case]
fn k_dev_install_checkIncorrect_invalidIrq() {
    log("k_dev_install_checkIncorrect_invalidIrq start\n\0");

    k_dev_install_preconditions!(device_id, dev_desc, create_valid_device_desc_minimal);

    dev_desc.driver.descriptor.irq = DEV_IRQ_LIMIT;
    unsafe {
        assert_eq!(k_dev_install(&device_id, &dev_desc), gnwDriverError::GDRE_IRQ_INVALID);
    }
    dev_desc.driver.descriptor.irq = DEV_IRQ_LIMIT + 1;
    unsafe {
        assert_eq!(k_dev_install(&device_id, &dev_desc), gnwDriverError::GDRE_IRQ_INVALID);
    }

    log("k_dev_install_checkIncorrect_invalidIrq end\n\0");
}

#[test_case]
fn k_dev_install_checkIncorrect_irqConflict() {
    log("k_dev_install_checkIncorrect_irqConflict start\n\0");

    k_dev_install_preconditions!(device_id, dev_desc, create_valid_device_desc_minimal);

    extern "C" fn isr() {}
    dev_desc.driver.descriptor.isr = Some(isr);
    unsafe {
        isrReg[0].routine = Some(isr);
    }

    unsafe {
        
        assert_eq!(k_dev_install(&device_id, &dev_desc), gnwDriverError::GDRE_IRQ_CONFLICT);
    }
    
    log("k_dev_install_checkIncorrect_irqConflict end\n\0");
}

#[test_case]
fn k_dev_install_checkIncorrect_initFailure() {
    log("k_dev_install_checkIncorrect_initFailure start\n\0");

    k_dev_install_preconditions!(device_id, dev_desc, create_valid_device_desc_minimal);
    
    extern "C" fn init() -> bool { return false; }
    dev_desc.driver.descriptor.init = Some(init);

    unsafe {
        assert_eq!(k_dev_install(&device_id, &dev_desc), gnwDriverError::GDRE_UNINITIALIZED);
    }
    
    log("k_dev_install_checkIncorrect_initFailure end\n\0");
}

/*
    enum gnwDriverError k_dev_start(size_t id)
*/

macro_rules! k_dev_start_preconditions {
    ($device_id:ident) => {
        let $device_id: size_t = 0;
        install_dummy_device(&$device_id, false);
        assert_eq!($device_id, 0);
    };
}

fn k_dev_start_expect(device_id: size_t, error: gnwDriverError, kernel_panic: bool) {
    unsafe {
        assert_eq!(k_dev_start(device_id), error);
        assert_eq!(KERNEL_PANIC_FLAG, kernel_panic);
        KERNEL_PANIC_FLAG = false;
    }
}

#[test_case]
fn k_dev_start_checkCorrect() {
    log("k_dev_start_checkCorrect start\n\0");

    k_dev_start_preconditions!(device_id);

    k_dev_start_expect(device_id, gnwDriverError::GDRE_NONE, false);
    
    log("k_dev_start_checkCorrect end\n\0");
}

#[test_case]
fn k_dev_start_checkIncorrect_idInvalid() {
    log("k_dev_start_checkIncorrect_idInvalid start\n\0");

    k_dev_start_preconditions!(device_id);

    k_dev_start_expect(MAX_DEVICES, gnwDriverError::GDRE_UNKNOWN, false);
    k_dev_start_expect(MAX_DEVICES + 1, gnwDriverError::GDRE_UNKNOWN, false);
    
    log("k_dev_start_checkIncorrect_idInvalid end\n\0");
}

#[test_case]
fn k_dev_start_checkIncorrect_deviceNotInitialized() {
    log("k_dev_start_checkIncorrect_deviceNotInitialized start\n\0");

    k_dev_start_preconditions!(device_id);

    unsafe {
        devices[device_id as usize].initialized = false;
    }

    k_dev_start_expect(device_id, gnwDriverError::GDRE_UNKNOWN, true);
    
    log("k_dev_start_checkIncorrect_deviceNotInitialized end\n\0");
}

#[test_case]
fn k_dev_start_checkIncorrect_deviceStartFailed() {
    log("k_dev_start_checkIncorrect_deviceStartFailed start\n\0");

    k_dev_start_preconditions!(device_id);

    extern "C" fn start() -> bool { return false; }
    unsafe {
        devices[device_id as usize].desc.driver.descriptor.start = Some(start);
    }

    k_dev_start_expect(device_id, gnwDriverError::GDRE_START_FAILED, false);
    
    log("k_dev_start_checkIncorrect_deviceStartFailed end\n\0");
}

/*
    enum gnwDeviceError k_dev_getById(const size_t id, struct gnwDeviceUHADesc * const desc)
*/

macro_rules! k_dev_getById_preconditions {
    ($device_id:ident, $uha_descriptor:ident, $device_descriptor:ident, $expected_uha_descriptor:ident, $install:expr) => {
        let $device_id: size_t = 0;
        #[allow(unused)] let $uha_descriptor: gnwDeviceUHADesc = Default::default();
        let $device_descriptor = create_valid_device_desc_complex();
        #[allow(unused)] let mut $expected_uha_descriptor: gnwDeviceUHADesc = Default::default();
        unsafe {
            #[allow(unused_assignments)]
            $expected_uha_descriptor = uhaGetDesc($device_id, $device_descriptor.r#type, $device_descriptor.api);
        }
        if ($install) {
            install_device(&$device_id, $device_descriptor);
            assert_eq!($device_id, 0);
        }
    };
}

#[allow(non_snake_case)]
fn k_dev_getById_expect(device_id: size_t, uha_descriptor: Option<&gnwDeviceUHADesc>, error: gnwDeviceError, kernel_panic: bool) {
    unsafe {
        match uha_descriptor {
            Some(desc) => assert_eq!(k_dev_getById(device_id, desc), error),
            None       => assert_eq!(k_dev_getById(device_id, null()), error),
        }
        assert_eq!(KERNEL_PANIC_FLAG, kernel_panic);
        KERNEL_PANIC_FLAG = false;
    }
}

#[test_case]
fn k_dev_getById_checkCorrect() {
    log("k_dev_getById_checkCorrect start\n\0");

    k_dev_getById_preconditions!(device_id, uha_descriptor, device_descriptor, expected_uha_descriptor, true);
    
    k_dev_getById_expect(device_id, Some(&uha_descriptor), gnwDeviceError::GDE_NONE, false);
    assert_eq!(uha_descriptor, expected_uha_descriptor);
    
    log("k_dev_getById_checkCorrect end\n\0");
}

#[test_case]
fn k_dev_getById_checkIncorrect_deviceNotInstalled() {
    log("k_dev_getById_checkIncorrect_deviceNotInstalled start\n\0");

    k_dev_getById_preconditions!(device_id, uha_descriptor, device_descriptor, expected_uha_descriptor, false);

    k_dev_getById_expect(device_id, Some(&uha_descriptor), gnwDeviceError::GDE_UNKNOWN, true);
    
    log("k_dev_getById_checkIncorrect_deviceNotInstalled end\n\0");
}

#[test_case]
fn k_dev_getById_checkIncorrect_idInvalid() {
    log("k_dev_getById_checkIncorrect_idInvalid start\n\0");

    k_dev_getById_preconditions!(device_id, uha_descriptor, device_descriptor, expected_uha_descriptor, true);

    for did in INVALID_DEVICE_ID_LIST {
        k_dev_getById_expect(did, Some(&uha_descriptor), gnwDeviceError::GDE_UNKNOWN, true);
    }
    
    log("k_dev_getById_checkIncorrect_idInvalid end\n\0");
}

#[test_case]
fn k_dev_getById_checkIncorrect_descNull() {
    log("k_dev_getById_checkIncorrect_descNull start\n\0");

    k_dev_getById_preconditions!(device_id, _uha_descriptor, device_descriptor, expected_uha_descriptor, true);

    k_dev_getById_expect(device_id, None, gnwDeviceError::GDE_UNKNOWN, true);
    
    log("k_dev_getById_checkIncorrect_descNull end\n\0");
}

/*
    enum gnwDeviceError k_dev_getByType(const enum gnwDeviceType type, struct gnwDeviceUHADesc * const desc)
*/

macro_rules! k_dev_getByType_preconditions {
    ($device_id:ident, $uha_descriptor:ident, $device_descriptor:ident, $expected_uha_descriptor:ident, $complex:expr) => {
        let $device_id: size_t = 0;
        #[allow(unused)] let $uha_descriptor: gnwDeviceUHADesc = Default::default();
        let $device_descriptor = if ($complex) {
            create_valid_device_desc_complex()
        } else {
            create_valid_device_desc_minimal()
        };
        #[allow(unused)] let mut $expected_uha_descriptor: gnwDeviceUHADesc = Default::default();
        unsafe {
            #[allow(unused_assignments)]
            $expected_uha_descriptor = uhaGetDesc($device_id, $device_descriptor.r#type, $device_descriptor.api);
        }
        install_device(&$device_id, $device_descriptor);
        assert_eq!($device_id, 0);
    };
}

#[allow(non_snake_case)]
fn k_dev_getByType_expect(r#type: gnwDeviceType, 
                          uha_descriptor: Option<&gnwDeviceUHADesc>,
                          error: gnwDeviceError, 
                          expected_uha_descriptor: Option<gnwDeviceUHADesc>, 
                          kernel_panic: bool) {
    unsafe {
        match uha_descriptor {
            Some(desc) => {
                assert_eq!(k_dev_getByType(r#type as u32, desc), error);
                assert_eq!(*desc, expected_uha_descriptor.unwrap());
            },
            None => {
                assert_eq!(k_dev_getByType(r#type as u32, null()), error);
            },
        }
        
        assert_eq!(KERNEL_PANIC_FLAG, kernel_panic);
        KERNEL_PANIC_FLAG = false;
    }
}

#[test_case]
fn k_dev_getByType_checkCorrect_minimal() {
    log("k_dev_getByType_checkCorrect_minimal start\n\0");

    k_dev_getByType_preconditions!(device_id, uha_descriptor, device_descriptor, expected_uha_descriptor, false);

    k_dev_getByType_expect(gnwDeviceType::DEV_TYPE_FDC, Some(&uha_descriptor), gnwDeviceError::GDE_NOT_FOUND, Some(Default::default()), false);
    k_dev_getByType_expect(gnwDeviceType::DEV_TYPE_MEM, Some(&uha_descriptor), gnwDeviceError::GDE_NOT_FOUND, Some(Default::default()), false);
    k_dev_getByType_expect(gnwDeviceType::DEV_TYPE_SYSTEM, Some(&uha_descriptor), gnwDeviceError::GDE_NONE, Some(expected_uha_descriptor), false);
    
    log("k_dev_getByType_checkCorrect_minimal end\n\0");
}

#[test_case]
fn k_dev_getByType_checkCorrect_complex() {
    log("k_dev_getByType_checkCorrect_complex start\n\0");

    k_dev_getByType_preconditions!(device_id, uha_descriptor, device_descriptor, expected_uha_descriptor, true);

    k_dev_getByType_expect(gnwDeviceType::DEV_TYPE_FDC, Some(&uha_descriptor), gnwDeviceError::GDE_NONE, Some(expected_uha_descriptor), false);
    k_dev_getByType_expect(gnwDeviceType::DEV_TYPE_MEM, Some(&uha_descriptor), gnwDeviceError::GDE_NONE, Some(expected_uha_descriptor), false);
    k_dev_getByType_expect(gnwDeviceType::DEV_TYPE_SYSTEM, Some(&uha_descriptor), gnwDeviceError::GDE_NONE, Some(expected_uha_descriptor), false);
    
    log("k_dev_getByType_checkCorrect_complex end\n\0");
}

#[test_case]
fn k_dev_getByType_checkIncorrect_descNull() {
    log("k_dev_getByType_checkIncorrect_descNull start\n\0");

    k_dev_getByType_preconditions!(device_id, uha_descriptor, device_descriptor, expected_uha_descriptor, true);

    k_dev_getByType_expect(gnwDeviceType::DEV_TYPE_SYSTEM, None, gnwDeviceError::GDE_UNKNOWN, None, true);
    
    log("k_dev_getByType_checkIncorrect_descNull end\n\0");
}

/*
    enum gnwDeviceError k_dev_getUHAForId(const size_t id, struct gnwDeviceUHA * const uha)
*/

macro_rules! k_dev_getUHAForId_preconditions {
    ($device_id:ident, $device_uha:ident, $device_descriptor:ident, $install:expr) => {
        let $device_id: size_t = 0;
        #[allow(unused)] let $device_uha: gnwDeviceUHA = Default::default();
        let $device_descriptor = create_valid_device_desc_complex();
        if ($install) {
            install_device(&$device_id, $device_descriptor);
            assert_eq!($device_id, 0);
        }
    };
}

#[allow(non_snake_case)]
fn k_dev_getUHAForId_expect(device_id: size_t, 
                            device_uha: Option<&gnwDeviceUHA>,
                            error: gnwDeviceError, 
                            kernel_panic: bool) {
    unsafe {
        match device_uha {
            Some(uha) => {
                assert_eq!(k_dev_getUHAForId(device_id, uha), error);
            },
            None => {
                assert_eq!(k_dev_getUHAForId(device_id, null()), error);
            },
        }

        assert_eq!(KERNEL_PANIC_FLAG, kernel_panic);
        KERNEL_PANIC_FLAG = false;
    }
}

#[test_case]
fn k_dev_getUHAForId_checkCorrect() {
    log("k_dev_getUHAForId_checkCorrect start\n\0");

    k_dev_getUHAForId_preconditions!(device_id, device_uha, device_descriptor, true);

    let expected_uha = device_descriptor.api;
    k_dev_getUHAForId_expect(device_id, Some(&device_uha), gnwDeviceError::GDE_NONE, false);
    assert_eq!(device_uha, expected_uha);

    log("k_dev_getUHAForId_checkCorrect end\n\0");
}

#[test_case]
fn k_dev_getUHAForId_checkIncorrect_deviceNotInstalled() {
    log("k_dev_getUHAForId_checkIncorrect_deviceNotInstalled start\n\0");

    k_dev_getUHAForId_preconditions!(device_id, device_uha, device_descriptor, false);

    k_dev_getUHAForId_expect(device_id, Some(&device_uha), gnwDeviceError::GDE_UNKNOWN, true);
    
    log("k_dev_getUHAForId_checkIncorrect_deviceNotInstalled end\n\0");
}

#[test_case]
fn k_dev_getUHAForId_checkIncorrect_idInvalid() {
    log("k_dev_getUHAForId_checkIncorrect_idInvalid start\n\0");

    k_dev_getUHAForId_preconditions!(device_id, device_uha, device_descriptor, true);

    for did in INVALID_DEVICE_ID_LIST {
        k_dev_getUHAForId_expect(did, Some(&device_uha), gnwDeviceError::GDE_UNKNOWN, true);
    }
    
    log("k_dev_getUHAForId_checkIncorrect_idInvalid end\n\0");
}

#[test_case]
fn k_dev_getUHAForId_checkIncorrect_descNull() {
    log("k_dev_getUHAForId_checkIncorrect_descNull start\n\0");

    k_dev_getUHAForId_preconditions!(device_id, device_uha, device_descriptor, true);

    k_dev_getUHAForId_expect(device_id, None, gnwDeviceError::GDE_UNKNOWN, true);
    
    log("k_dev_getUHAForId_checkIncorrect_descNull end\n\0");
}

/*
    enum gnwDeviceError k_dev_acquireHold(const procId_t processId, const size_t deviceId)
*/

macro_rules! k_dev_acquireHold_preconditions {
    ($device_id:ident, $proc_id:ident, $install_device:expr, $install_process:expr) => {
        let $device_id: size_t = 0;
        let $proc_id: procId_t = if ($install_process) {
            install_dummy_process()
        } else { 0 };
        assert_eq!($proc_id, 0);
        if ($install_device) {
            install_dummy_device(&$device_id, true);
            assert_eq!($device_id, 0);
        }
    };
}

#[allow(non_snake_case)]
fn k_dev_acquireHold_expect(device_id: size_t, 
                            proc_id: procId_t,
                            error: gnwDeviceError, 
                            kernel_panic: bool) {
    unsafe {
        assert_eq!(k_dev_acquireHold(proc_id, device_id), error);
        assert_eq!(KERNEL_PANIC_FLAG, kernel_panic);
        KERNEL_PANIC_FLAG = false;
    }
}

#[test_case]
fn k_dev_acquireHold_checkCorrect() {
    log("k_dev_acquireHold_checkCorrect start\n\0");

    k_dev_acquireHold_preconditions!(device_id, proc_id, true, true);

    k_dev_acquireHold_expect(device_id, proc_id, gnwDeviceError::GDE_NONE, false);
    unsafe {
        assert_eq!(devices[device_id as usize].holder, proc_id);
    }
    
    log("k_dev_acquireHold_checkCorrect end\n\0");
}

#[test_case]
fn k_dev_acquireHold_checkIncorrect_deviceNotInstalled() {
    log("k_dev_acquireHold_checkIncorrect_deviceNotInstalled start\n\0");

    k_dev_acquireHold_preconditions!(device_id, proc_id, false, true);
    
    for did in DEVICE_ID_LIST {
        k_dev_acquireHold_expect(did, proc_id, gnwDeviceError::GDE_UNKNOWN, true);
    }

    log("k_dev_acquireHold_checkIncorrect_deviceNotInstalled end\n\0");
}

#[test_case]
fn k_dev_acquireHold_checkIncorrect_deviceIdInvalid() {
    log("k_dev_acquireHold_checkIncorrect_deviceIdInvalid start\n\0");

    k_dev_acquireHold_preconditions!(device_id, proc_id, true, true);
    
    for did in INVALID_DEVICE_ID_LIST {
        k_dev_acquireHold_expect(did, proc_id, gnwDeviceError::GDE_UNKNOWN, true);
    }

    log("k_dev_acquireHold_checkIncorrect_deviceIdInvalid end\n\0");
}

#[test_case]
fn k_dev_acquireHold_checkIncorrect_processNotExisting() {
    log("k_dev_acquireHold_checkIncorrect_processNotExisting start\n\0");

    k_dev_acquireHold_preconditions!(device_id, proc_id, true, false);

    for pid in PID_LIST {
        k_dev_acquireHold_expect(device_id, pid, gnwDeviceError::GDE_UNKNOWN, true);
    }
    
    log("k_dev_acquireHold_checkIncorrect_processNotExisting end\n\0");
}

#[test_case]
fn k_dev_acquireHold_checkIncorrect_processIdInvalid() {
    log("k_dev_acquireHold_checkIncorrect_processIdInvalid start\n\0");

    k_dev_acquireHold_preconditions!(device_id, proc_id, true, true);

    for pid in INVALID_PID_LIST {
        k_dev_acquireHold_expect(device_id, pid, gnwDeviceError::GDE_UNKNOWN, true);
    }

    log("k_dev_acquireHold_checkIncorrect_processIdInvalid end\n\0");
}

#[test_case]
fn k_dev_acquireHold_checkIncorrect_alreadyHeld() {
    log("k_dev_acquireHold_checkIncorrect_alreadyHeld start\n\0");

    k_dev_acquireHold_preconditions!(device_id, proc_id, true, true);

    unsafe {
        devices[device_id as usize].holder = 0;
    }
    k_dev_acquireHold_expect(device_id, proc_id, gnwDeviceError::GDE_ALREADY_HELD, false);
    unsafe {
        devices[device_id as usize].holder = 1;
    }
    k_dev_acquireHold_expect(device_id, proc_id, gnwDeviceError::GDE_ALREADY_HELD, false);
    
    log("k_dev_acquireHold_checkIncorrect_alreadyHeld end\n\0");
}

/*
    void k_dev_releaseHold(const procId_t processId, const size_t deviceId)
*/

macro_rules! k_dev_releaseHold_preconditions {
    ($device_id:ident, $proc_id:ident, $install_device:expr, $install_holder:expr) => {
        let $device_id: size_t = 0;
        let $proc_id: procId_t = install_dummy_process();
        assert_eq!($proc_id, 0);

        if ($install_device) {
            install_dummy_device(&$device_id, true);
            assert_eq!($device_id, 0);
        }
        if ($install_holder) {
            install_dummy_device_holder($device_id, $proc_id);
        }
    };
}

#[allow(non_snake_case)]
fn k_dev_releaseHold_expect(device_id: size_t, 
                            proc_id: procId_t, 
                            kernel_panic: bool) {
    unsafe {
        k_dev_releaseHold(proc_id, device_id);
        assert_eq!(KERNEL_PANIC_FLAG, kernel_panic);
        KERNEL_PANIC_FLAG = false;
    }
}

#[test_case]
fn k_dev_releaseHold_checkCorrect() {
    log("k_dev_releaseHold_checkCorrect start\n\0");

    k_dev_releaseHold_preconditions!(device_id, proc_id, true, true);
    
    k_dev_releaseHold_expect(device_id, proc_id, false);
    
    unsafe {
        assert_eq!(devices[device_id as usize].holder, NONE_PROC_ID);
        assert_eq!(devices[device_id as usize].listener, None);
        assert_eq!(devices[device_id as usize].decoder, None);
    }
    
    log("k_dev_releaseHold_checkCorrect end\n\0");
}

#[test_case]
fn k_dev_releaseHold_checkIncorrect_deviceNotInstalled() {
    log("k_dev_releaseHold_checkIncorrect_deviceNotInstalled start\n\0");

    k_dev_releaseHold_preconditions!(device_id, proc_id, false, false);

    for did in DEVICE_ID_LIST {
        k_dev_releaseHold_expect(did, proc_id, true);
    }
    
    log("k_dev_releaseHold_checkIncorrect_deviceNotInstalled end\n\0");
}

#[test_case]
fn k_dev_releaseHold_checkIncorrect_deviceIdInvalid() {
    log("k_dev_releaseHold_checkIncorrect_deviceIdInvalid start\n\0");

    k_dev_releaseHold_preconditions!(device_id, proc_id, true, true);

    for did in INVALID_DEVICE_ID_LIST {
        k_dev_releaseHold_expect(did, proc_id, true);
    }
    
    log("k_dev_releaseHold_checkIncorrect_deviceIdInvalid end\n\0");
}

#[test_case]
fn k_dev_releaseHold_checkIncorrect_processIdInvalid() {
    log("k_dev_releaseHold_checkIncorrect_deviceIdInvalid start\n\0");
    
    k_dev_releaseHold_preconditions!(device_id, proc_id, true, true);
    
    unsafe {
        let expected_device = devices[device_id as usize];
        for pid in INVALID_PID_LIST {
            k_dev_releaseHold_expect(device_id, pid, false);
            assert_eq!(devices[device_id as usize], expected_device);    
        }
    }
    
    log("k_dev_releaseHold_checkIncorrect_deviceIdInvalid end\n\0");
}

/*
    enum gnwDeviceError k_dev_writeMem(const procId_t processId, 
                                       const size_t deviceId,
                                       const ptr_t absBuffer,
                                       const range_addr_t devMemRange)
*/

macro_rules! k_dev_writeMem_preconditions {
    ($device_id:ident, $proc_id:ident, $buffer:ident, $input_range:ident) => {
        let $device_id: size_t = 0;
        let mut $proc_id: procId_t = 0;
        install_dummy_writable_device(&$device_id, &mut $proc_id);
        #[allow(unused, unused_mut)] let mut $buffer: u8 = 0;
        let $input_range = range_addr_t {
            offset: 0,
            sizeBytes: 1,
        };

        unsafe {
            devices[0].started = true;
        }
    };
}

#[allow(non_snake_case)]
fn k_dev_writeMem_expect(device_id: size_t,
                         proc_id: procId_t,
                         buffer: ptr_t,
                         input_range: range_addr_t,
                         error: gnwDeviceError,
                         kernel_panic: bool,
                         write_called: bool) {
    unsafe {
        assert_eq!(k_dev_writeMem(proc_id, device_id, buffer, input_range), error);
        assert_eq!(KERNEL_PANIC_FLAG, kernel_panic);
        KERNEL_PANIC_FLAG = false;
    }

    if kernel_panic {
        return;
    }

    if write_called {
        unsafe {
            assert_eq!(DEV_WRITE_CALLED, true);
            assert_eq!(DEV_WRITE_PARAM_BUFFER, buffer);
            assert_eq!(DEV_WRITE_PARAM_RANGE, input_range);
            DEV_WRITE_CALLED = false;
            DEV_WRITE_PARAM_BUFFER = null_mut();
            DEV_WRITE_PARAM_RANGE = Default::default();
        }
    }
}

#[test_case]
fn k_dev_writeMem_checkCorrect() {
    log("k_dev_writeMem_checkCorrect start\n\0");

    k_dev_writeMem_preconditions!(device_id, proc_id, buffer, input_range);

    k_dev_writeMem_expect(device_id, proc_id, &mut buffer, input_range, gnwDeviceError::GDE_NONE, false, true);
    
    log("k_dev_writeMem_checkCorrect end\n\0");
}

#[test_case]
fn k_dev_writeMem_checkIncorrect_nullBuffer() {
    log("k_dev_writeMem_checkIncorrect_nullBuffer start\n\0");

    k_dev_writeMem_preconditions!(device_id, proc_id, buffer, input_range);

    k_dev_writeMem_expect(device_id, proc_id, null_mut(), input_range, gnwDeviceError::GDE_UNKNOWN, true, false);
    
    log("k_dev_writeMem_checkIncorrect_nullBuffer end\n\0");
}

#[test_case]
fn k_dev_writeMem_checkIncorrect_deviceNotInstalled() {
    log("k_dev_writeMem_checkIncorrect_deviceNotInstalled start\n\0");
    
    k_dev_writeMem_preconditions!(device_id, proc_id, buffer, input_range);

    unsafe {
        devicesCount = 0;
    }

    k_dev_writeMem_expect(device_id, proc_id, &mut buffer, input_range, gnwDeviceError::GDE_UNKNOWN, false, false);
    
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
        for did in INVALID_DEVICE_ID_LIST {
            assert_eq!(k_dev_writeMem(proc_id, did, &mut buffer, input_range), gnwDeviceError::GDE_UNKNOWN);
        }
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
        for pid in INVALID_PID_LIST {
            assert_eq!(k_dev_writeMem(pid, id, &mut buffer, input_range), gnwDeviceError::GDE_UNKNOWN);
            assert_eq!(KERNEL_PANIC_FLAG, true);
            KERNEL_PANIC_FLAG = false;
        }
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
        for did in INVALID_DEVICE_ID_LIST {
            assert_eq!(k_dev_writeChar(proc_id, did, 0), gnwDeviceError::GDE_UNKNOWN);
        }
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
        for pid in INVALID_PID_LIST {
            assert_eq!(k_dev_writeChar(pid, id, 0), gnwDeviceError::GDE_UNKNOWN);
            assert_eq!(KERNEL_PANIC_FLAG, true);
            KERNEL_PANIC_FLAG = false;
        }
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
        for did in INVALID_DEVICE_ID_LIST {
            assert_eq!(k_dev_listen(proc_id, did, Some(listener), Some(decoder)), gnwDeviceError::GDE_UNKNOWN);
        }
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
        for pid in INVALID_PID_LIST {
            assert_eq!(k_dev_listen(pid, device_id, Some(listener), Some(decoder)), gnwDeviceError::GDE_UNKNOWN);
            assert_eq!(KERNEL_PANIC_FLAG, true);
            KERNEL_PANIC_FLAG = false;
        }
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
        for did in INVALID_DEVICE_ID_LIST {
            assert_eq!(k_dev_getParam(did, param_descriptor, &abs_result), gnwDeviceError::GDE_ID_INVALID);
        }
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
        for did in INVALID_DEVICE_ID_LIST {
            assert_eq!(k_dev_setParam(proc_id, did, param_descriptor, 0), gnwDeviceError::GDE_UNKNOWN);
        }
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
        for pid in INVALID_PID_LIST {
            assert_eq!(k_dev_setParam(pid, device_id, param_descriptor, 0), gnwDeviceError::GDE_UNKNOWN);
            assert_eq!(KERNEL_PANIC_FLAG, true);
            KERNEL_PANIC_FLAG = false;
        }
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

    let mut device_id: size_t = 0;
    install_dummy_device(&device_id, true);
    assert_eq!(device_id, 0);

    unsafe {
        for did in INVALID_DEVICE_ID_LIST {
            device_id = did;
            assert_eq!(validateEmitter(&did), gnwDeviceError::GDE_UNKNOWN);
            assert_eq!(did, device_id);
            assert_eq!(KERNEL_PANIC_FLAG, true);
            KERNEL_PANIC_FLAG = false;
        }
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

macro_rules! k_dev_emit_preconditions {
    ($device_id:ident, $proc_id:ident, $data:ident, $event:ident) => {
        let mut $device_id: size_t = 0;
        let mut $proc_id: procId_t = 0;
        let mut $data: u8 = 96;
        let mut $event: gnwDeviceEvent = gnwDeviceEvent {
            r#type: 69,
            data: null_mut(),
            dataSizeBytes: 1,
        };
        $event.data = &mut $data;
        install_dummy_emitter_device(&mut $device_id, &mut $proc_id);
    };
}

fn k_dev_emit_expect(event: &gnwDeviceEvent, error_code: gnwDeviceError, kernel_panic: bool) {
    unsafe {
        assert_eq!(k_dev_emit(event), error_code);
        assert_eq!(KERNEL_PANIC_FLAG, kernel_panic);
        KERNEL_PANIC_FLAG = false;
    }
}

#[test_case]
fn k_dev_emit_checkCorrect() {
    log("k_dev_emit_checkCorrect start\n\0");

    k_dev_emit_preconditions!(device_id, proc_id, data, event);
    
    unsafe {
        let index: u32 = 1;
        let expected_queue_decoded_data: &[u8] = from_raw_parts((&event as *const gnwDeviceEvent) as *const u8, mem::size_of::<gnwDeviceEvent>());
        
        k_dev_emit_expect(&event, gnwDeviceError::GDE_NONE, false);

        assert_eq!(data, 96);
        assert_eq!(rlp_main[proc_id as usize].endIndex, index);
        assert_eq!(rlp_main[proc_id as usize].finishedIndex, index - 1);
        assert_eq!(rlp_main[proc_id as usize].queue[index as usize].reserved, true);
        assert_eq!(rlp_main[proc_id as usize].queue[index as usize].handled, false);
        assert_eq!(rlp_main[proc_id as usize].queue[index as usize].dataHandled, false);
        assert_eq!(rlp_main[proc_id as usize].queue[index as usize].item.format, gnwEventFormat::GEF_PTR);
        assert_eq!(rlp_main[proc_id as usize].queue[index as usize].item.routine._ptr.unwrap() as ptr_t, dev_event_listener as ptr_t);
        assert_eq!(rlp_main[proc_id as usize].queue[index as usize].item.dataSizeBytes, 13);
        assert_eq!(rlp_main[proc_id as usize].queue[index as usize].item.decodedDataSizeBytes, 12);
        assert_eq!(rlp_main[proc_id as usize].queue[index as usize].item.decode.unwrap() as ptr_t, dev_event_decoder as ptr_t);
        for byte_offset in 0..rlp_main[proc_id as usize].queue[index as usize].item.decodedDataSizeBytes {
            assert_eq!(rlp_main[proc_id as usize].queue[index as usize].data[byte_offset as usize], expected_queue_decoded_data[byte_offset as usize]);
        }
        assert_eq!(rlp_main[proc_id as usize].queue[index as usize].data[rlp_main[proc_id as usize].queue[index as usize].item.decodedDataSizeBytes as usize], data);
        for byte_offset in rlp_main[proc_id as usize].queue[index as usize].item.dataSizeBytes..DISPATCH_MAX_DATA_SIZE_BYTES {
            assert_eq!(rlp_main[proc_id as usize].queue[index as usize].data[byte_offset as usize], 0);
        }
        assert_eq!(pTab[proc_id as usize].lockMask, k_proc_lockType::PLT_IPC as i32);
        assert_eq!(executionTimeCounter, GRANULARITY_MS);
    }
    
    log("k_dev_emit_checkCorrect end\n\0");
}

#[test_case]
fn k_dev_emit_checkIncorrect_eventPtrNull() {
    log("k_dev_emit_checkIncorrect_eventPtrNull start\n\0");

    k_dev_emit_preconditions!(device_id, proc_id, data, event);
    
    unsafe {
        assert_eq!(k_dev_emit(null()), gnwDeviceError::GDE_UNKNOWN);
        assert_eq!(KERNEL_PANIC_FLAG, true);
    }
    
    log("k_dev_emit_checkIncorrect_eventPtrNull end\n\0");
}

#[test_case]
fn k_dev_emit_checkIncorrect_devIdPtrNull() {
    log("k_dev_emit_checkIncorrect_devIdPtrNull start\n\0");

    k_dev_emit_preconditions!(device_id, proc_id, data, event);
    
    unsafe {
        k_hal_servicedDevIdPtr = null();
    }

    k_dev_emit_expect(&event, gnwDeviceError::GDE_UNKNOWN, true);
    
    log("k_dev_emit_checkIncorrect_devIdPtrNull end\n\0");
}

#[test_case]
fn k_dev_emit_checkIncorrect_deviceIdInvalid() {
    log("k_dev_emit_checkIncorrect_deviceIdInvalid start\n\0");

    k_dev_emit_preconditions!(device_id, proc_id, data, event);

    for did in INVALID_DEVICE_ID_LIST {
       unsafe {
            k_hal_servicedDevIdPtr = &did;
        }
        k_dev_emit_expect(&event, gnwDeviceError::GDE_UNKNOWN, true);
    }
    
    log("k_dev_emit_checkIncorrect_deviceIdInvalid end\n\0");
}

#[test_case]
fn k_dev_emit_checkIncorrect_deviceNotStarted() {
    log("k_dev_emit_checkIncorrect_deviceNotStarted start\n\0");

    k_dev_emit_preconditions!(device_id, proc_id, data, event);
    
    unsafe {
        devices[device_id as usize].started = false;
    }

    k_dev_emit_expect(&event, gnwDeviceError::GDE_INVALID_DEVICE_STATE, false);
    
    log("k_dev_emit_checkIncorrect_deviceNotStarted end\n\0");
}

#[test_case]
fn k_dev_emit_checkIncorrect_listenerNull() {
    log("k_dev_emit_checkIncorrect_listenerNull start\n\0");

    k_dev_emit_preconditions!(device_id, proc_id, data, event);
    
    unsafe {
        devices[device_id as usize].listener = None;
    }

    k_dev_emit_expect(&event, gnwDeviceError::GDE_NOT_FOUND, false);
    
    log("k_dev_emit_checkIncorrect_listenerNull end\n\0");
}

#[test_case]
fn k_dev_emit_checkIncorrect_decoderNull() {
    log("k_dev_emit_checkIncorrect_decoderNull start\n\0");

    k_dev_emit_preconditions!(device_id, proc_id, data, event);
    
    unsafe {
        devices[device_id as usize].decoder = None;
    }

    k_dev_emit_expect(&event, gnwDeviceError::GDE_UNKNOWN, true);
    
    log("k_dev_emit_checkIncorrect_decoderNull end\n\0");
}

#[test_case]
fn k_dev_emit_checkIncorrect_holderProcessIdInvalid() {
    log("k_dev_emit_checkIncorrect_holderProcessIdInvalid start\n\0");

    k_dev_emit_preconditions!(device_id, proc_id, data, event);

    unsafe {
        for pid in INVALID_PID_LIST {
            devices[device_id as usize].holder = pid;
            k_dev_emit_expect(&event, gnwDeviceError::GDE_UNKNOWN, true);
        }
    }
    
    log("k_dev_emit_checkIncorrect_holderProcessIdInvalid end\n\0");
}

#[test_case]
fn k_dev_emit_checkIncorrect_holderProcessFinished() {
    log("k_dev_emit_checkIncorrect_holderProcessFinished start\n\0");

    k_dev_emit_preconditions!(device_id, proc_id, data, event);

    unsafe {
        pTab[proc_id as usize].info.state = k_proc_state::PS_FINISHED;
    }

    k_dev_emit_expect(&event, gnwDeviceError::GDE_HANDLE_INVALID, false);

    log("k_dev_emit_checkIncorrect_holderProcessFinished end\n\0");
}

#[test_case]
fn k_dev_emit_checkIncorrect_runLoopFull() {
    log("k_dev_emit_checkIncorrect_runLoopFull start\n\0");

    k_dev_emit_preconditions!(device_id, proc_id, data, event);

    unsafe {
        rlp_main[proc_id as usize].queue[1].reserved = true;
    }

    k_dev_emit_expect(&event, gnwDeviceError::GDE_UNKNOWN, false);

    log("k_dev_emit_checkIncorrect_runLoopFull end\n\0");
}

#[test_case]
fn k_dev_emit_checkIncorrect_eventDataTooLarge() {
    log("k_dev_emit_checkIncorrect_eventDataTooLarge start\n\0");

    k_dev_emit_preconditions!(device_id, proc_id, data, event);

    event.dataSizeBytes = DISPATCH_MAX_DATA_SIZE_BYTES - (core::mem::size_of::<gnwDeviceEvent>() as u32) + 1;

    k_dev_emit_expect(&event, gnwDeviceError::GDE_UNKNOWN, true);

    log("k_dev_emit_checkIncorrect_eventDataTooLarge end\n\0");
}

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