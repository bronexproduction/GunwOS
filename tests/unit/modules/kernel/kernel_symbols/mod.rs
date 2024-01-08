use core::ffi::c_char;

extern "C" {
    pub fn k_log_log(szMsg: *const c_char);
    pub fn k_bus_outb(port: u16, val: u8);
}
