use core::ffi::c_char;
use kernel_symbols::k_log_log;

pub fn log(msg: &str) {
    unsafe {
        k_log_log(msg.as_ptr() as *const c_char);
    }
}
