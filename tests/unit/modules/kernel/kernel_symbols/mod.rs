use core::ffi::c_char;

extern "C" {
    pub fn k_purge();
    pub fn k_log_log(szMsg: *const c_char);
    
    // dev

    pub static maxDevices: u32;
    pub fn validateId(id: u32) -> bool;
}
