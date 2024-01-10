use core::ffi::c_char;

#[allow(non_camel_case_types)]
type procId_t = i32;

#[allow(non_camel_case_types)]
type size_t = u32;

#[allow(non_camel_case_types)]
type ptr_t = *mut u8;

#[allow(non_camel_case_types)]
type addr_t = u32;

#[allow(non_camel_case_types)]
struct range_addr_t {
    offset: addr_t,
    sizeBytes: size_t,
}

#[repr(packed)]
#[allow(non_camel_case_types)]
pub struct gnwStorCHS {
    c: u16,
    h: u8,
    s: u8,
}

#[allow(non_camel_case_types)]
pub struct gnwStorGeometry {
    chs: gnwStorCHS,
    lba: size_t,
    sectSizeBytes: u16,
}

#[repr(i32)]
#[allow(non_camel_case_types)]
pub enum gnwStorErrorCode {
    GSEC_NONE = 0,
    GSEC_DRIVE_NOT_PRESENT,
    GSEC_MEDIA_NOT_PRESENT,
    GSEC_API_UNAVAILABLE,
    GSEC_INVALID_FORMAT,
    GSEC_COMMAND_FAILED,
    GSEC_BUFFER_OVERFLOW,
    GSEC_INVALID_ARGUMENT,
    GSEC_UNKNOWN = -1
}

#[allow(non_camel_case_types)]
pub struct gnwStorError {
    code: gnwStorErrorCode,
    internalCode: u32,
}

#[repr(i32)]
#[allow(non_camel_case_types)]
pub enum gnwDeviceType {
    DEV_TYPE_SYSTEM     = (1 << 0),
    DEV_TYPE_MEM        = (1 << 1),
    DEV_TYPE_KEYBOARD   = (1 << 2),
    DEV_TYPE_MOUSE      = (1 << 3),
    DEV_TYPE_DISPLAY    = (1 << 4),
    DEV_TYPE_CHAR_IN    = (1 << 5),
    DEV_TYPE_CHAR_OUT   = (1 << 6),
    DEV_TYPE_STORAGE    = (1 << 7),
    DEV_TYPE_FDC        = (1 << 8) | gnwDeviceType::DEV_TYPE_STORAGE as i32,
    DEV_TYPE_NONE       = 0
}

#[allow(non_camel_case_types)]
pub struct gnwDeviceUHA_system_desc {
    _unused: u32,
}

#[allow(non_camel_case_types)]
pub struct gnwDeviceUHA_system_routine {
    getParam: fn(param: size_t,
                 subParam: size_t,
                 paramIndex: size_t,
                 result: *mut size_t) -> bool,
    setParam: fn(param: size_t,
                 subParam: size_t,
                 paramIndex: size_t,
                 value: size_t) -> bool,
}

#[allow(non_camel_case_types)]
pub struct gnwDeviceUHA_system {
    desc: gnwDeviceUHA_system_desc,
    routine: gnwDeviceUHA_system_routine,
}

#[allow(non_camel_case_types)]
pub struct gnwDeviceUHA_mem_desc {
    bytesRange: range_addr_t,
    maxInputSizeBytes: size_t,
}

#[allow(non_camel_case_types)]
pub struct gnwDeviceUHA_mem_routine {
    write: fn(buffer: ptr_t, inputBufferRange: range_addr_t),
}

#[allow(non_camel_case_types)]
pub struct gnwDeviceUHA_mem {
    desc: gnwDeviceUHA_mem_desc,
    routine: gnwDeviceUHA_mem_routine,
}

#[allow(non_camel_case_types)]
pub struct gnwDeviceUHA_keyboard_desc {
    _unused: u32,
}

#[allow(non_camel_case_types)]
pub struct gnwDeviceUHA_keyboard_routine {
    _unused: fn(),
}

#[allow(non_camel_case_types)]
pub struct gnwDeviceUHA_keyboard {
    desc: gnwDeviceUHA_keyboard_desc,
    routine: gnwDeviceUHA_keyboard_routine,
}

#[allow(non_camel_case_types)]
pub struct gnwDeviceUHA_mouse_desc {
    _unused: u32,
}

#[allow(non_camel_case_types)]
pub struct gnwDeviceUHA_mouse_routine {
    _unused: fn(),
}

#[allow(non_camel_case_types)]
pub struct gnwDeviceUHA_mouse {
    desc: gnwDeviceUHA_mouse_desc,
    routine: gnwDeviceUHA_mouse_routine,
}

#[allow(non_camel_case_types)]
pub struct gnwDeviceUHA_fdc_desc {
    _unused: u32,
}

#[allow(non_camel_case_types)]
pub struct gnwDeviceUHA_fdc_routine {
    _unused: fn(),
}

#[allow(non_camel_case_types)]
pub struct gnwDeviceUHA_fdc {
    desc: gnwDeviceUHA_fdc_desc,
    routine: gnwDeviceUHA_fdc_routine,
}

#[allow(non_camel_case_types)]
pub struct gnwDeviceUHA_storCtrl_desc {
    driveCount: u8,
    removable: bool,
    removableMedia: bool,
}

#[allow(non_camel_case_types)]
pub struct gnwDeviceUHA_storCtrl_routine {
    drivePresent: fn(index: u8) -> bool,
    driveGeometry: fn(index: u8) -> gnwStorGeometry,
    read: fn(index: u8,
             lba: size_t,
             count: size_t,
             buffer: *const u8,
             error: *mut gnwStorError) -> size_t,
}

#[allow(non_camel_case_types)]
pub struct gnwDeviceUHA_storCtrl {
    desc: gnwDeviceUHA_storCtrl_desc,
    routine: gnwDeviceUHA_storCtrl_routine,
}

#[allow(non_camel_case_types)]
pub struct gnwDeviceUHA_charIn_desc {
    _unused: u32,
}

#[allow(non_camel_case_types)]
pub struct gnwDeviceUHA_charOut_desc {
    _unused: u32,
}

#[allow(non_camel_case_types)]
pub struct gnwDeviceUHA_charIn_routine {
    hasData: fn() -> bool,
    read: fn(c: *const u8) -> size_t,
}

#[allow(non_camel_case_types)]
pub struct gnwDeviceUHA_charOut_routine {
    isReadyToWrite: fn() -> bool,
    write: fn(c: char) -> bool,
}

#[allow(non_camel_case_types)]
pub struct gnwDeviceUHA_charIn {
    desc: gnwDeviceUHA_charIn_desc,
    routine: gnwDeviceUHA_charIn_routine,
}

#[allow(non_camel_case_types)]
pub struct gnwDeviceUHA_charOut {
    desc: gnwDeviceUHA_charOut_desc,
    routine: gnwDeviceUHA_charOut_routine,
}

#[allow(non_camel_case_types)]
pub struct gnwDeviceUHA_display_desc {
    supportedFormatCount: size_t,
}

#[allow(non_camel_case_types)]
pub struct gnwDeviceUHA_display_routine {
    _unused: fn(),
}

#[allow(non_camel_case_types)]
pub struct gnwDeviceUHA_display {
    desc: gnwDeviceUHA_display_desc,
    routine: gnwDeviceUHA_display_routine,
}

#[allow(non_camel_case_types)]
pub struct gnwDeviceUHA_event_desc {
    _unused: u32,
}

#[allow(non_camel_case_types)]
pub struct gnwDeviceUHA_event_routine {
    _unused: fn(),
}

#[allow(non_camel_case_types)]
pub struct gnwDeviceUHA_event {
    desc: gnwDeviceUHA_event_desc,
    routine: gnwDeviceUHA_event_routine,
}

#[allow(non_camel_case_types)]
pub struct gnwDeviceUHA {
    system: gnwDeviceUHA_system,      // DEV_TYPE_SYSTEM
    mem: gnwDeviceUHA_mem,            // DEV_TYPE_MEM
    keyboard: gnwDeviceUHA_keyboard,  // DEV_TYPE_KEYBOARD
    mouse: gnwDeviceUHA_mouse,        // DEV_TYPE_MOUSE
    fdc: gnwDeviceUHA_fdc,            // DEV_TYPE_FDC
    storCtrl: gnwDeviceUHA_storCtrl,  // DEV_TYPE_STORAGE
    charIn: gnwDeviceUHA_charIn,      // DEV_TYPE_CHAR_IN
    charOut: gnwDeviceUHA_charOut,    // DEV_TYPE_CHAR_OUT
    display: gnwDeviceUHA_display,    // DEV_TYPE_DISPLAY
    event: gnwDeviceUHA_event,        // event emitting devices
}

#[allow(non_camel_case_types)]
pub struct gnwDriverConfig {
    init: fn() -> bool,
    start: fn() -> bool,
    isr: fn(),
    irq: u8,
}
    
#[allow(non_camel_case_types)]
pub struct gnwDeviceIO {
    busBase: u16,
}

#[allow(non_camel_case_types)]
pub struct gnwDeviceDriver {
    io: gnwDeviceIO,
    descriptor: gnwDriverConfig,
}

#[allow(non_camel_case_types)]
pub struct gnwDeviceDescriptor {
    r#type: gnwDeviceType,
    api: gnwDeviceUHA,
    driver: gnwDeviceDriver,
    name: *const u8,
}

#[allow(non_camel_case_types)]
pub struct gnwDeviceEvent {
    r#type: u32,
    data: ptr_t,
    dataSizeBytes: size_t,
}

#[allow(non_camel_case_types)]
type gnwDeviceEventListener = extern "cdecl" fn(event: *const gnwDeviceEvent);

#[allow(non_camel_case_types)]
type gnwDeviceEventDecoder = fn(ptr_t, *const gnwDeviceEvent);

#[allow(non_camel_case_types)]
struct device {
    desc: gnwDeviceDescriptor,
    initialized: bool,
    started: bool,
    holder: procId_t,
    listener: gnwDeviceEventListener,
    decoder: gnwDeviceEventDecoder,
}

#[repr(i32)]
#[allow(non_camel_case_types)]
#[derive(PartialEq, Debug)]
pub enum gnwDeviceError {
    GDE_NONE = 0,
    GDE_BUSY,
    GDE_ID_INVALID,
    GDE_NOT_FOUND,
    GDE_CANNOT_BE_HELD,
    GDE_ALREADY_HELD,
    GDE_ALREADY_SET,
    GDE_HANDLE_INVALID,
    GDE_LISTENER_INVALID,
    GDE_DECODER_INVALID,
    GDE_INVALID_DEVICE_STATE,
    GDE_INVALID_OPERATION,
    GDE_INVALID_PARAMETER,
    GDE_PRECONDITION_NOT_SATISFIED,
    GDE_OPERATION_FAILED,
    GDE_UNKNOWN = -1
}

pub const MAX_DEVICES: size_t = 8;

extern "C" {
    pub fn k_purge();
    pub fn k_log_log(szMsg: *const c_char);
    
    // dev

    pub static mut devicesCount: u32;
    pub static mut devices: [device; MAX_DEVICES as usize];
    pub fn validateId(id: size_t) -> bool;
    pub fn validateInstalledId(id: size_t) -> bool;
    pub fn validateStartedDevice(procId: procId_t, devId: size_t) -> gnwDeviceError;
}
