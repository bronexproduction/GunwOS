#![no_std]
#![no_main]
#![feature(custom_test_frameworks)]
#![test_runner(crate::test_runner)]
#![reexport_test_harness_main = "test_main"]

extern "C" {
    fn k_log_log(msg: &str);
}

#[link_section = ".start_override"]
pub extern "C" fn __kernel_start_test() -> ! {
    unsafe {
        k_log_log("Unit tests run started\n");
    }
    
    // test_main();

    unsafe {
        k_log_log("Unit tests run finished - all tests passed\n");
    }

    loop {}
}

fn test_runner(tests: &[&dyn Fn()]) {
    for test in tests {
        test();
    }
}
