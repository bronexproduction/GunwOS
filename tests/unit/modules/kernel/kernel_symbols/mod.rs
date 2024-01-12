#![allow(non_camel_case_types)]
#![allow(non_snake_case)]
#![allow(dead_code)]

use core::ffi::c_char;

pub type procId_t = i32;
pub type size_t = u32;
pub type ptr_t = *mut u8;
pub type addr_t = u32;

#[repr(C)]
pub struct range_addr_t {
    pub offset: addr_t,
    pub sizeBytes: size_t,
}

#[repr(C, packed)]
pub struct gnwStorCHS {
    pub c: u16,
    pub h: u8,
    pub s: u8,
}

#[repr(C)]
pub struct gnwStorGeometry {
    pub chs: gnwStorCHS,
    pub lba: size_t,
    pub sectSizeBytes: u16,
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
    pub code: gnwStorErrorCode,
    pub internalCode: u32,
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
    pub _unused: u32,
}

#[repr(C)]
pub struct gnwDeviceUHA_system_routine {
    pub getParam: Option<extern "C" fn(param: size_t,
                                       subParam: size_t,
                                       paramIndex: size_t,
                                       result: *mut size_t) -> bool>,
    pub setParam: Option<extern "C" fn(param: size_t,
                                       subParam: size_t,
                                       paramIndex: size_t,
                                       value: size_t) -> bool>,
}

#[repr(C)]
pub struct gnwDeviceUHA_system {
    pub desc: gnwDeviceUHA_system_desc,
    pub routine: gnwDeviceUHA_system_routine,
}

#[repr(C)]
pub struct gnwDeviceUHA_mem_desc {
    pub bytesRange: range_addr_t,
    pub maxInputSizeBytes: size_t,
}

#[repr(C)]
pub struct gnwDeviceUHA_mem_routine {
    pub write: Option<extern "C" fn(buffer: ptr_t, inputBufferRange: range_addr_t)>,
}

#[repr(C)]
pub struct gnwDeviceUHA_mem {
    pub desc: gnwDeviceUHA_mem_desc,
    pub routine: gnwDeviceUHA_mem_routine,
}

#[repr(C)]
pub struct gnwDeviceUHA_keyboard_desc {
    pub _unused: u32,
}

#[repr(C)]
pub struct gnwDeviceUHA_keyboard_routine {
    pub _unused: Option<extern "C" fn()>,
}

#[repr(C)]
pub struct gnwDeviceUHA_keyboard {
    pub desc: gnwDeviceUHA_keyboard_desc,
    pub routine: gnwDeviceUHA_keyboard_routine,
}

#[repr(C)]
pub struct gnwDeviceUHA_mouse_desc {
    pub _unused: u32,
}

#[repr(C)]
pub struct gnwDeviceUHA_mouse_routine {
    pub _unused: Option<extern "C" fn()>,
}

#[repr(C)]
pub struct gnwDeviceUHA_mouse {
    pub desc: gnwDeviceUHA_mouse_desc,
    pub routine: gnwDeviceUHA_mouse_routine,
}

#[repr(C)]
pub struct gnwDeviceUHA_fdc_desc {
    pub _unused: u32,
}

#[repr(C)]
pub struct gnwDeviceUHA_fdc_routine {
    pub _unused: Option<extern "C" fn()>,
}

#[repr(C)]
pub struct gnwDeviceUHA_fdc {
    pub desc: gnwDeviceUHA_fdc_desc,
    pub routine: gnwDeviceUHA_fdc_routine,
}

#[repr(C)]
pub struct gnwDeviceUHA_storCtrl_desc {
    pub driveCount: u8,
    pub removable: bool,
    pub removableMedia: bool,
}

#[repr(C)]
pub struct gnwDeviceUHA_storCtrl_routine {
    pub drivePresent: Option<extern "C" fn(index: u8) -> bool>,
    pub driveGeometry: Option<extern "C" fn(index: u8) -> gnwStorGeometry>,
    pub read: Option<extern "C" fn(index: u8,
                                   lba: size_t,
                                   count: size_t,
                                   buffer: *const u8,
                                   error: *mut gnwStorError) -> size_t>,
}

#[repr(C)]
pub struct gnwDeviceUHA_storCtrl {
    pub desc: gnwDeviceUHA_storCtrl_desc,
    pub routine: gnwDeviceUHA_storCtrl_routine,
}

#[repr(C)]
pub struct gnwDeviceUHA_charIn_desc {
    pub _unused: u32,
}

#[repr(C)]
pub struct gnwDeviceUHA_charOut_desc {
    pub _unused: u32,
}

#[repr(C)]
pub struct gnwDeviceUHA_charIn_routine {
    pub hasData: Option<extern "C" fn() -> bool>,
    pub read: Option<extern "C" fn(c: *const u8) -> size_t>,
}

#[repr(C)]
pub struct gnwDeviceUHA_charOut_routine {
    pub isReadyToWrite: Option<extern "C" fn() -> bool>,
    pub write: Option<extern "C" fn(c: c_char) -> bool>,
}

#[repr(C)]
pub struct gnwDeviceUHA_charIn {
    pub desc: gnwDeviceUHA_charIn_desc,
    pub routine: gnwDeviceUHA_charIn_routine,
}

#[repr(C)]
pub struct gnwDeviceUHA_charOut {
    pub desc: gnwDeviceUHA_charOut_desc,
    pub routine: gnwDeviceUHA_charOut_routine,
}

#[repr(C)]
pub struct gnwDeviceUHA_display_desc {
    pub supportedFormatCount: size_t,
}

#[repr(C)]
pub struct gnwDeviceUHA_display_routine {
    pub _unused: Option<extern "C" fn()>,
}

#[repr(C)]
pub struct gnwDeviceUHA_display {
    pub desc: gnwDeviceUHA_display_desc,
    pub routine: gnwDeviceUHA_display_routine,
}

#[repr(C)]
pub struct gnwDeviceUHA_event_desc {
    pub _unused: u32,
}

#[repr(C)]
pub struct gnwDeviceUHA_event_routine {
    pub _unused: Option<extern "C" fn()>,
}

#[repr(C)]
pub struct gnwDeviceUHA_event {
    pub desc: gnwDeviceUHA_event_desc,
    pub routine: gnwDeviceUHA_event_routine,
}

#[repr(C)]
pub struct gnwDeviceUHA {
    pub system: gnwDeviceUHA_system,      // DEV_TYPE_SYSTEM
    pub mem: gnwDeviceUHA_mem,            // DEV_TYPE_MEM
    pub keyboard: gnwDeviceUHA_keyboard,  // DEV_TYPE_KEYBOARD
    pub mouse: gnwDeviceUHA_mouse,        // DEV_TYPE_MOUSE
    pub fdc: gnwDeviceUHA_fdc,            // DEV_TYPE_FDC
    pub storCtrl: gnwDeviceUHA_storCtrl,  // DEV_TYPE_STORAGE
    pub charIn: gnwDeviceUHA_charIn,      // DEV_TYPE_CHAR_IN
    pub charOut: gnwDeviceUHA_charOut,    // DEV_TYPE_CHAR_OUT
    pub display: gnwDeviceUHA_display,    // DEV_TYPE_DISPLAY
    pub event: gnwDeviceUHA_event,        // event emitting devices
}

#[repr(C)]
pub struct gnwDriverConfig {
    pub init: Option<extern "C" fn() -> bool>,
    pub start: Option<extern "C" fn() -> bool>,
    pub isr: Option<extern "C" fn()>,
    pub irq: u8,
}

#[repr(C)]
pub struct gnwDeviceIO {
    pub busBase: u16,
}

#[repr(C)]
pub struct gnwDeviceDriver {
    pub io: gnwDeviceIO,
    pub descriptor: gnwDriverConfig,
}

#[repr(C)]
pub struct gnwDeviceDescriptor {
    pub r#type: i32, /* gnwDeviceType bitmask */
    pub api: gnwDeviceUHA,
    pub driver: gnwDeviceDriver,
    pub name: *const u8,
}

#[repr(C)]
pub struct gnwDeviceEvent {
    pub r#type: u32,
    pub data: ptr_t,
    pub dataSizeBytes: size_t,
}

pub type gnwDeviceEventListener = Option<extern "cdecl" fn(event: *const gnwDeviceEvent)>;
pub type gnwDeviceEventDecoder = Option<extern "C" fn(ptr_t, *const gnwDeviceEvent)>;

#[repr(C)]
pub struct device {
    pub desc: gnwDeviceDescriptor,
    pub initialized: bool,
    pub started: bool,
    pub holder: procId_t,
    pub listener: gnwDeviceEventListener,
    pub decoder: gnwDeviceEventDecoder,
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
    GDE_UNKNOWN = -1,
}

#[repr(i32)]
#[derive(PartialEq, Debug)]
pub enum gnwDriverError {
    NO_ERROR        = 0,
    UNINITIALIZED   = 1,
    START_FAILED    = 2,
    IRQ_CONFLICT    = 3,
    IRQ_INVALID     = 4,
    LIMIT_REACHED   = 5,
    ISR_MISSING     = 6,
    UNKNOWN         = -1,
}

pub const NONE_PROC_ID: procId_t = -2;
pub const KERNEL_PROC_ID: procId_t = -1;

pub const DEV_IRQ_LIMIT: u8 = 8;
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
    pub fn k_dev_install(id: *const size_t, descriptor: *const gnwDeviceDescriptor) -> gnwDriverError;
}
