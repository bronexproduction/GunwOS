#![no_std]
#![no_main]
#![feature(custom_test_frameworks)]
#![test_runner(kernel_test_runner)]
#![reexport_test_harness_main = "test_main"]
#![feature(stmt_expr_attributes)]

mod kernel_symbols;
mod kernel_tests;
mod utils;

use core::panic::PanicInfo;
use kernel_symbols::k_purge;
use utils::*;

use kernel_tests::dev::helpers::dev_clear;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
#[repr(u8)]
enum QemuExitCode {
    Success = 0x22,
    Failed = 0x23,
}

#[used]
static TEST_ENTRY: extern "C" fn() -> ! = _test_entry;
#[link_section = ".start"]
#[no_mangle]
pub extern "C" fn _test_entry() -> ! {
    log("Unit tests run started\n\0");
    test_main();
    log("Unit tests run finished - all tests passed\n\0");
    
    exit_qemu(QemuExitCode::Success);

    loop {}
}

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    log("panic while running tests\n\0");

    exit_qemu(QemuExitCode::Failed);

    loop {}
}

fn exit_qemu(exit_code: QemuExitCode) {
    unsafe {
        bus_outb(0xf4, exit_code as u8);
    }
}

fn kernel_test_runner(tests: &[&dyn Fn()]) {
    for test in tests {
        unsafe {
            cpu_cli();
            k_purge();
            KERNEL_PANIC_FLAG = false;
            dev_clear();
        }
        test();
    }
}
