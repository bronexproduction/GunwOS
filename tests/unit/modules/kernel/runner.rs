#![no_std]
#![no_main]
#![feature(custom_test_frameworks)]
#![test_runner(kernel_test_runner)]
#![reexport_test_harness_main = "test_main"]

mod kernel_symbols;
mod kernel_tests;
mod utils;

use core::panic::PanicInfo;
use utils::log;

#[used]
static TEST_ENTRY: extern "C" fn() -> ! = __kernel_start_test;

#[link_section = ".start_override"]
pub extern "C" fn __kernel_start_test() -> ! {
    log("Unit tests run started\n\0");

    test_main();

    log("Unit tests run finished - all tests passed\n\0");

    loop {}
}

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    log("panic while running tests\n\0");

    loop {}
}

fn kernel_test_runner(tests: &[&dyn Fn()]) {
    for test in tests {
        test();
    }
}
