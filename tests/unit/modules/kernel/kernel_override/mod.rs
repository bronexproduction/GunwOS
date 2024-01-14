use core::ffi::c_char;
use utils::*;

#[used]
static TEST_K_OOPS: extern "cdecl" fn(*const c_char, u32) = k_oops;
#[no_mangle]
pub extern "cdecl" fn k_oops(_reason: *const c_char, _code: u32) {
    unsafe {
        KERNEL_PANIC_FLAG = true;
    }
}