use core::ffi::c_char;

#[allow(non_camel_case_types)]
type procId_t = i32;

#[allow(non_camel_case_types)]
type size_t = u32;

#[repr(i32)]
#[allow(non_camel_case_types)]
enum gnwDeviceError {
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

extern "C" {
    pub fn k_purge();
    pub fn k_log_log(szMsg: *const c_char);
    
    // dev

    pub static maxDevices: u32;
    pub static mut devicesCount: u32;
    pub fn validateId(id: size_t) -> bool;
    pub fn validateInstalledId(id: size_t) -> bool;
    pub fn validateStartedDevice(procId: procId_t, devId: size_t) -> gnwDeviceError;
}
