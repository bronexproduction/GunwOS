use utils::*;

#[test_case]
fn test_test_case() {
    log("test_test_case start\n\0");
    assert_eq!(1, 1);
    log("test_test_case end\n\0");
}

#[test_case]
fn test_test_srase() {
    log("test_test_srase start\n\0");
    assert_eq!(1, 1);
    log("test_test_srase end\n\0");
}