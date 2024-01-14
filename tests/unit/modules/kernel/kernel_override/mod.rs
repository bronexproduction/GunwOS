use core::ffi::c_char;
use utils::KERNEL_PANIC_FLAG;

pub extern "cdecl" fn k_oops(_reason: *const c_char, _code: u32) {
    unsafe {
        KERNEL_PANIC_FLAG = true;
    }
}