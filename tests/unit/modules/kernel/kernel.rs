#![no_std]
#![no_main]

use core::panic::PanicInfo;
use core::intrinsics::transmute;
use core::ffi::c_char;

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}

#[used]
static TEST___KERNEL_START: extern "C" fn() -> ! = __kernel_start;
#[link_section = ".start"]
#[no_mangle]
pub extern "C" fn __kernel_start() -> ! {
    let ptr = 0x00400000 as *const ();
    let test_entry: extern "C" fn() -> ! = unsafe { transmute(ptr) };
    (test_entry)();
}

#[no_mangle]
pub static mut KERNEL_PANIC_FLAG: bool = false;

#[used]
static TEST_K_OOPS: extern "cdecl" fn(*const c_char, u32) = k_oops;
#[no_mangle]
pub extern "cdecl" fn k_oops(_reason: *const c_char, _code: u32) {
    unsafe {
        KERNEL_PANIC_FLAG = true;
    }
}