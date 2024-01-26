use kernel_symbols::*;
use utils::KERNEL_PANIC_FLAG;
use core::ptr::null_mut;

pub static mut DRIVER_INIT_CALLED: bool = false;
pub static mut DRIVER_START_CALLED: bool = false;
pub static mut DEV_WRITE_CALLED: bool = false;
pub static mut DEV_WRITE_PARAM_BUFFER: ptr_t = null_mut();
pub static mut DEV_WRITE_PARAM_RANGE: range_addr_t = range_addr_t {
    offset: 0,
    sizeBytes: 0,
};
pub static mut DEV_CHAR_WRITE_CALLED: bool = false;
pub static mut DEV_CHAR_WRITE_PARAM_CHARACTER: i8 = 0;
pub static mut DEV_GET_PARAM_CALLED: bool = false;
pub static mut DEV_SET_PARAM_CALLED: bool = false;
pub static mut DEV_EVENT_LISTENER_CALLED: bool = false;
pub static mut DEV_EVENT_DECODER_CALLED: bool = false;

pub fn dev_clear() {
    unsafe {
        DRIVER_INIT_CALLED = false;
        DRIVER_START_CALLED = false;
        DEV_WRITE_CALLED = false;
        DEV_WRITE_PARAM_BUFFER = null_mut();
        DEV_WRITE_PARAM_RANGE = Default::default();
        DEV_CHAR_WRITE_CALLED = false;
        DEV_CHAR_WRITE_PARAM_CHARACTER = 0;
        DEV_GET_PARAM_CALLED = false;
        DEV_SET_PARAM_CALLED = false;
        DEV_EVENT_LISTENER_CALLED = false;
        DEV_EVENT_DECODER_CALLED = false;
    }
}

pub fn create_empty_device_desc() -> gnwDeviceDescriptor {
    let system_api_desc = gnwDeviceUHA_system_desc {
        _unused: 0,
    };
    let system_api_routine = gnwDeviceUHA_system_routine {
        getParam: None,
        setParam: None,
    };
    let system_api = gnwDeviceUHA_system {
        desc: system_api_desc,
        routine: system_api_routine,
    };
    let mem_api_desc = gnwDeviceUHA_mem_desc {
        bytesRange: range_addr_t {
            offset: 0,
            sizeBytes: 0,
        },
        maxInputSizeBytes: 0,
    };
    let mem_api_routine = gnwDeviceUHA_mem_routine {
        write: None,
    };
    let mem_api = gnwDeviceUHA_mem {
        desc: mem_api_desc,
        routine: mem_api_routine,
    };
    let keyboard_api_desc = gnwDeviceUHA_keyboard_desc {
        _unused: 0,
    };
    let keyboard_api_routine = gnwDeviceUHA_keyboard_routine {
        _unused: None,
    };
    let keyboard_api = gnwDeviceUHA_keyboard {
        desc: keyboard_api_desc,
        routine: keyboard_api_routine,
    };
    let mouse_api_desc = gnwDeviceUHA_mouse_desc {
        _unused: 0,
    };
    let mouse_api_routine = gnwDeviceUHA_mouse_routine {
        _unused: None,
    };
    let mouse_api = gnwDeviceUHA_mouse {
        desc: mouse_api_desc,
        routine: mouse_api_routine,
    };
    let fdc_api_desc = gnwDeviceUHA_fdc_desc {
        _unused: 0,
    };
    let fdc_api_routine = gnwDeviceUHA_fdc_routine {
        _unused: None,
    };
    let fdc_api = gnwDeviceUHA_fdc {
        desc: fdc_api_desc,
        routine: fdc_api_routine,
    };
    let stor_ctrl_api_desc = gnwDeviceUHA_storCtrl_desc {
        driveCount: 0,
        removable: false,
        removableMedia: false,
    };
    let stor_ctrl_api_routine = gnwDeviceUHA_storCtrl_routine {
        drivePresent: None,
        driveGeometry: None,
        read: None,
    };
    let stor_ctrl_api = gnwDeviceUHA_storCtrl {
        desc: stor_ctrl_api_desc,
        routine: stor_ctrl_api_routine,
    };
    let char_in_api_desc = gnwDeviceUHA_charIn_desc {
        _unused: 0,
    };
    let char_in_api_routine = gnwDeviceUHA_charIn_routine {
        hasData: None,
        read: None,
    };
    let char_in_api = gnwDeviceUHA_charIn {
        desc: char_in_api_desc,
        routine: char_in_api_routine,
    };
    let char_out_api_desc = gnwDeviceUHA_charOut_desc {
        _unused: 0,
    };
    let char_out_api_routine = gnwDeviceUHA_charOut_routine {
        isReadyToWrite: None,
        write: None,
    };
    let char_out_api = gnwDeviceUHA_charOut {
        desc: char_out_api_desc,
        routine: char_out_api_routine,
    };
    let display_api_desc = gnwDeviceUHA_display_desc {
        supportedFormatCount: 0,
    };
    let display_api_routine = gnwDeviceUHA_display_routine {
        _unused: None,
    };
    let display_api = gnwDeviceUHA_display {
        desc: display_api_desc,
        routine: display_api_routine,
    };
    let event_api_desc = gnwDeviceUHA_event_desc {
        _unused: 0,
    };
    let event_api_routine = gnwDeviceUHA_event_routine {
        _unused: None,
    };
    let event_api = gnwDeviceUHA_event {
        desc: event_api_desc,
        routine: event_api_routine,
    };
    let api = gnwDeviceUHA {
        system: system_api,
        mem: mem_api,
        keyboard: keyboard_api,
        mouse: mouse_api,
        fdc: fdc_api,
        storCtrl: stor_ctrl_api,
        charIn: char_in_api,
        charOut: char_out_api,
        display: display_api,
        event: event_api,
    };
    let io = gnwDeviceIO {
        busBase: 0x00,
    };
    let driver_config = gnwDriverConfig {
        init: None,
        start: None,
        isr: None,
        irq: 0x00,
    };
    let driver = gnwDeviceDriver {
        io: io,
        descriptor: driver_config,
    };
    let name: *const u8 = "Dummy device\0".as_ptr();
    let descriptor = gnwDeviceDescriptor {
        r#type: gnwDeviceType::DEV_TYPE_NONE as i32,
        api: api,
        driver: driver,
        name: name as *const u8,
    };

    return descriptor;
}

pub fn create_valid_device_desc_minimal() -> gnwDeviceDescriptor {
    let mut device_descriptor = create_empty_device_desc();
    device_descriptor.r#type = gnwDeviceType::DEV_TYPE_SYSTEM as i32;

    return device_descriptor;
}

pub fn create_valid_device_desc_complex() -> gnwDeviceDescriptor {
    let mut device_descriptor = create_empty_device_desc();
    device_descriptor.r#type = gnwDeviceType::DEV_TYPE_SYSTEM as i32;

    device_descriptor.r#type = gnwDeviceType::DEV_TYPE_SYSTEM as i32   |
                               gnwDeviceType::DEV_TYPE_MEM as i32      |
                               gnwDeviceType::DEV_TYPE_KEYBOARD as i32 |
                               gnwDeviceType::DEV_TYPE_MOUSE as i32    |
                               gnwDeviceType::DEV_TYPE_DISPLAY as i32  |
                               gnwDeviceType::DEV_TYPE_CHAR_IN as i32  |
                               gnwDeviceType::DEV_TYPE_CHAR_OUT as i32 |
                               gnwDeviceType::DEV_TYPE_STORAGE as i32  |
                               gnwDeviceType::DEV_TYPE_FDC as i32;

    extern "C" fn system_get_param(_: u32, _: u32, _: u32, _: *mut u32) -> bool { 
        unsafe {
            DEV_GET_PARAM_CALLED = true;
        }
        return true;
    }
    device_descriptor.api.system.routine.getParam = Some(system_get_param);
    extern "C" fn system_set_param(_: u32, _: u32, _: u32, _: u32) -> bool {
        unsafe {
            DEV_SET_PARAM_CALLED = true;
        }
        return true;
    }
    device_descriptor.api.system.routine.setParam = Some(system_set_param);

    device_descriptor.api.mem.desc.bytesRange = range_addr_t {
        offset: 0x69,
        sizeBytes: 0x69,
    };
    extern "C" fn mem_write(buffer: *mut u8, range: range_addr_t) {
        unsafe {
            DEV_WRITE_CALLED = true;
            DEV_WRITE_PARAM_BUFFER = buffer;
            DEV_WRITE_PARAM_RANGE = range; 
        }
    }
    device_descriptor.api.mem.routine.write = Some(mem_write);

    device_descriptor.api.display.desc.supportedFormatCount = 1;

    extern "C" fn char_in_has_data() -> bool { return false; }
    device_descriptor.api.charIn.routine.hasData = Some(char_in_has_data);
    extern "C" fn char_in_read(_: *const u8) -> u32 { return 0; }
    device_descriptor.api.charIn.routine.read = Some(char_in_read);

    extern "C" fn char_out_is_ready_to_write() -> bool { return true; }
    device_descriptor.api.charOut.routine.isReadyToWrite = Some(char_out_is_ready_to_write);
    extern "C" fn char_out_write(character: i8) -> bool { 
        unsafe {
            DEV_CHAR_WRITE_CALLED = true;
            DEV_CHAR_WRITE_PARAM_CHARACTER = character;
        }
        return true;
    }
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
    extern "C" fn start() -> bool {
        unsafe {
            DRIVER_START_CALLED = true;
        }
        return true;
    }
    device_descriptor.driver.descriptor.start = Some(start);

    return device_descriptor;
}

pub fn install_dummy_device(id: &size_t, complex: bool) {
    let device_descriptor = if complex {
        create_valid_device_desc_complex()
    } else { 
        create_valid_device_desc_minimal()
    };

    install_device(id, device_descriptor);
}

pub fn install_device(id: &size_t, descriptor: gnwDeviceDescriptor) {
    unsafe {
        k_dev_install(id, &descriptor);
        devices[*id as usize].started = true;
    }
}

pub fn install_dummy_process() -> procId_t {
    unsafe {
        pTab[0].info.state = k_proc_state::PS_RUNNING;
    }
    return 0;
}

pub fn install_dummy_device_holder(device_id: size_t, proc_id: procId_t) {
    unsafe {
        devices[device_id as usize].holder = proc_id;
    }
}

pub extern "cdecl" fn dev_event_listener(_: *const gnwDeviceEvent) {
    unsafe {
        DEV_EVENT_LISTENER_CALLED = true;
    }
}

pub extern "C" fn dev_event_decoder(_: *mut u8, _: *const gnwDeviceEvent) {
    unsafe {
        DEV_EVENT_DECODER_CALLED = true;
    }
}

pub fn install_dummy_device_listener(device_id: size_t, proc_id: procId_t) {
    install_dummy_device_holder(device_id, proc_id);

    unsafe {
        devices[device_id as usize].listener = Some(dev_event_listener);
        devices[device_id as usize].decoder = Some(dev_event_decoder);
    }
}

pub fn install_dummy_writable_device(id: &size_t, proc_id: &mut procId_t) {
    *proc_id = install_dummy_process();
    let mut device_descriptor = create_valid_device_desc_complex();
    let device_range = range_addr_t {
        offset: 1,
        sizeBytes: 1,
    };
    device_descriptor.api.mem.desc.bytesRange = device_range;
    device_descriptor.api.mem.desc.maxInputSizeBytes = 1;
    install_device(&id, device_descriptor);
    assert_eq!(*id, 0);
    assert_eq!(*proc_id, 0);
    install_dummy_device_holder(*id, *proc_id);
}

pub fn install_dummy_emitter_device(device_id: &mut size_t, proc_id: &mut procId_t) {
    *proc_id = install_dummy_process();
    assert_eq!(*proc_id, 0);
    install_dummy_device(device_id, true);
    assert_eq!(*device_id, 0);
    install_dummy_device_listener(*device_id, *proc_id);
    
    unsafe {
        isrStackHeight = 1;
        pTab[0].lockMask = k_proc_lockType::PLT_EVENT as i32 |
                           k_proc_lockType::PLT_IPC as i32;
        k_hal_servicedDevIdPtr = device_id;
        queueRunning = true;
        assert_eq!(KERNEL_PANIC_FLAG, false);
    }
}