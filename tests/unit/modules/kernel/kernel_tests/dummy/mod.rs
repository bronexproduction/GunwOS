use kernel_symbols::*;

#[test_case]
fn test_test_case() {
    unsafe {
        k_log_log("test_test_case start\n");
    }
    assert_eq!(1, 1);
    unsafe {
        k_log_log("test_test_case end\n");
    }
}

#[test_case]
fn test_test_srase() {
    unsafe {
        k_log_log("test_test_srase start\n");
    }
    assert_eq!(1, 1);
    unsafe {
        k_log_log("test_test_srase end\n");
    }
}