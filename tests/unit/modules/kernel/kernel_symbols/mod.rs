#![allow(non_camel_case_types)]
#![allow(non_snake_case)]
#![allow(dead_code)]

use core::ffi::c_char;

type procId_t = i32;
type size_t = u32;
type ptr_t = *mut u8;
type addr_t = u32;

#[repr(C)]
struct range_addr_t {
    offset: addr_t,
    sizeBytes: size_t,
}

#[repr(C, packed)]
pub struct gnwStorCHS {
    c: u16,
    h: u8,
    s: u8,
}

#[repr(C)]
pub struct gnwStorGeometry {
    chs: gnwStorCHS,
    lba: size_t,
    sectSizeBytes: u16,
}

#[repr(i32)]
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

#[repr(C)]
pub struct gnwStorError {
    code: gnwStorErrorCode,
    internalCode: u32,
}

#[repr(i32)]
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

#[repr(C)]
pub struct gnwDeviceUHA_system_desc {
    _unused: u32,
}

#[repr(C)]
pub struct gnwDeviceUHA_system_routine {
    getParam: extern "C" fn(param: size_t,
                            subParam: size_t,
                            paramIndex: size_t,
                            result: *mut size_t) -> bool,
    setParam: extern "C" fn(param: size_t,
                            subParam: size_t,
                            paramIndex: size_t,
                            value: size_t) -> bool,
}

#[repr(C)]
pub struct gnwDeviceUHA_system {
    desc: gnwDeviceUHA_system_desc,
    routine: gnwDeviceUHA_system_routine,
}

#[repr(C)]
pub struct gnwDeviceUHA_mem_desc {
    bytesRange: range_addr_t,
    maxInputSizeBytes: size_t,
}

#[repr(C)]
pub struct gnwDeviceUHA_mem_routine {
    write: extern "C" fn(buffer: ptr_t, inputBufferRange: range_addr_t),
}

#[repr(C)]
pub struct gnwDeviceUHA_mem {
    desc: gnwDeviceUHA_mem_desc,
    routine: gnwDeviceUHA_mem_routine,
}

#[repr(C)]
pub struct gnwDeviceUHA_keyboard_desc {
    _unused: u32,
}

#[repr(C)]
pub struct gnwDeviceUHA_keyboard_routine {
    _unused: extern "C" fn(),
}

#[repr(C)]
pub struct gnwDeviceUHA_keyboard {
    desc: gnwDeviceUHA_keyboard_desc,
    routine: gnwDeviceUHA_keyboard_routine,
}

#[repr(C)]
pub struct gnwDeviceUHA_mouse_desc {
    _unused: u32,
}

#[repr(C)]
pub struct gnwDeviceUHA_mouse_routine {
    _unused: extern "C" fn(),
}

#[repr(C)]
pub struct gnwDeviceUHA_mouse {
    desc: gnwDeviceUHA_mouse_desc,
    routine: gnwDeviceUHA_mouse_routine,
}

#[repr(C)]
pub struct gnwDeviceUHA_fdc_desc {
    _unused: u32,
}

#[repr(C)]
pub struct gnwDeviceUHA_fdc_routine {
    _unused: extern "C" fn(),
}

#[repr(C)]
pub struct gnwDeviceUHA_fdc {
    desc: gnwDeviceUHA_fdc_desc,
    routine: gnwDeviceUHA_fdc_routine,
}

#[repr(C)]
pub struct gnwDeviceUHA_storCtrl_desc {
    driveCount: u8,
    removable: bool,
    removableMedia: bool,
}

#[repr(C)]
pub struct gnwDeviceUHA_storCtrl_routine {
    drivePresent: extern "C" fn(index: u8) -> bool,
    driveGeometry: extern "C" fn(index: u8) -> gnwStorGeometry,
    read: extern "C" fn(index: u8,
                        lba: size_t,
                        count: size_t,
                        buffer: *const u8,
                        error: *mut gnwStorError) -> size_t,
}

#[repr(C)]
pub struct gnwDeviceUHA_storCtrl {
    desc: gnwDeviceUHA_storCtrl_desc,
    routine: gnwDeviceUHA_storCtrl_routine,
}

#[repr(C)]
pub struct gnwDeviceUHA_charIn_desc {
    _unused: u32,
}

#[repr(C)]
pub struct gnwDeviceUHA_charOut_desc {
    _unused: u32,
}

#[repr(C)]
pub struct gnwDeviceUHA_charIn_routine {
    hasData: extern "C" fn() -> bool,
    read: extern "C" fn(c: *const u8) -> size_t,
}

#[repr(C)]
pub struct gnwDeviceUHA_charOut_routine {
    isReadyToWrite: extern "C" fn() -> bool,
    write: extern "C" fn(c: c_char) -> bool,
}

#[repr(C)]
pub struct gnwDeviceUHA_charIn {
    desc: gnwDeviceUHA_charIn_desc,
    routine: gnwDeviceUHA_charIn_routine,
}

#[repr(C)]
pub struct gnwDeviceUHA_charOut {
    desc: gnwDeviceUHA_charOut_desc,
    routine: gnwDeviceUHA_charOut_routine,
}

#[repr(C)]
pub struct gnwDeviceUHA_display_desc {
    supportedFormatCount: size_t,
}

#[repr(C)]
pub struct gnwDeviceUHA_display_routine {
    _unused: extern "C" fn(),
}

#[repr(C)]
pub struct gnwDeviceUHA_display {
    desc: gnwDeviceUHA_display_desc,
    routine: gnwDeviceUHA_display_routine,
}

#[repr(C)]
pub struct gnwDeviceUHA_event_desc {
    _unused: u32,
}

#[repr(C)]
pub struct gnwDeviceUHA_event_routine {
    _unused: extern "C" fn(),
}

#[repr(C)]
pub struct gnwDeviceUHA_event {
    desc: gnwDeviceUHA_event_desc,
    routine: gnwDeviceUHA_event_routine,
}

#[repr(C)]
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

#[repr(C)]
pub struct gnwDriverConfig {
    init: extern "C" fn() -> bool,
    start: extern "C" fn() -> bool,
    isr: extern "C" fn(),
    irq: u8,
}

#[repr(C)]
pub struct gnwDeviceIO {
    busBase: u16,
}

#[repr(C)]
pub struct gnwDeviceDriver {
    io: gnwDeviceIO,
    descriptor: gnwDriverConfig,
}

#[repr(C)]
pub struct gnwDeviceDescriptor {
    r#type: gnwDeviceType,
    api: gnwDeviceUHA,
    driver: gnwDeviceDriver,
    name: *const u8,
}

#[repr(C)]
pub struct gnwDeviceEvent {
    r#type: u32,
    data: ptr_t,
    dataSizeBytes: size_t,
}

type gnwDeviceEventListener = extern "cdecl" fn(event: *const gnwDeviceEvent);
type gnwDeviceEventDecoder = extern "C" fn(ptr_t, *const gnwDeviceEvent);

#[repr(C)]
pub struct device {
    desc: gnwDeviceDescriptor,
    initialized: bool,
    pub started: bool,
    pub holder: procId_t,
    listener: gnwDeviceEventListener,
    decoder: gnwDeviceEventDecoder,
}

#[repr(i32)]
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

pub const NONE_PROC_ID: procId_t = -2;
pub const KERNEL_PROC_ID: procId_t = -1;

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
