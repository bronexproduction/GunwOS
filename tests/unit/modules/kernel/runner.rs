#![no_std]
#![feature(custom_test_frameworks)]
#![test_runner(crate::test_runner)]

#[no_mangle]
pub fn __kernel_start() {
    test_runner();
}

pub fn test_runner(tests: &[&dyn Fn()]) {
    for test in tests {
        test();
    }
}
