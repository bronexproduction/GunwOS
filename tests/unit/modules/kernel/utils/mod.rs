use core::ffi::c_char;
use kernel_symbols::k_log_log;

extern "C" {
    pub fn outb(port: u16, val: u8);
}

pub fn log(msg: &str) {
    unsafe {
        k_log_log(msg.as_ptr() as *const c_char);
    }
}
