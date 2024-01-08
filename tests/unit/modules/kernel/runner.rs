#![no_std]
#![no_main]
#![feature(custom_test_frameworks)]
#![test_runner(kernel_test_runner)]
#![reexport_test_harness_main = "test_main"]

mod kernel_symbols;
mod kernel_tests;

use core::panic::PanicInfo;
use kernel_symbols::k_log_log;

#[used]
static TEST_ENTRY: extern "C" fn() -> ! = __kernel_start_test;

#[link_section = ".start_override"]
pub extern "C" fn __kernel_start_test() -> ! {
    unsafe {
        k_log_log("Unit tests run started\n");
    }

    test_main();

    unsafe {
        k_log_log("Unit tests run finished - all tests passed\n");
    }

    loop {}
}

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    unsafe {
        k_log_log("panic while running tests")
    }

    loop {}
}

fn kernel_test_runner(tests: &[&dyn Fn()]) {
    for test in tests {
        test();
    }
}
