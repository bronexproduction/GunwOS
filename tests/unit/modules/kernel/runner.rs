#![no_std]
#![no_main]
#![feature(custom_test_frameworks)]
#![test_runner(crate::test_runner)]
#![reexport_test_harness_main = "test_main"]

#[no_mangle]
pub extern "C" fn __kernel_start() -> ! {
    // test_main();

    loop {}
}

fn test_runner(tests: &[&dyn Fn()]) {
    for test in tests {
        test();
    }
}
