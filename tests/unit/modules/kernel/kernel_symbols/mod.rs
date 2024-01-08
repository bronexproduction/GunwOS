use core::ffi::c_char;

extern "C" {
    pub fn k_log_log(szMsg: *const c_char);
}
