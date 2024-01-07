#![no_std]
#![feature(custom_test_frameworks)]

#[test_case]
fn trivial_assertion() {
    assert_eq!(1, 1);
}
