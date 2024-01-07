#![no_std]
#![feature(custom_test_frameworks)]
#![test_runner(crate::test_runner)]

pub fn test_runner(tests: &[&dyn Fn()]) {
    for test in tests {
        test();
    }
}
