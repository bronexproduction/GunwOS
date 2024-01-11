use kernel_symbols::*;

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
        r#type: gnwDeviceType::DEV_TYPE_NONE,
        api: api,
        driver: driver,
        name: name as *const u8,
    };

    return descriptor;
}