
use utils::*;
use kernel_symbols::*;

#[test_case]
fn validateId_checkCorrect() {
    log("validateId_checkCorrect1 start\n\0");
    unsafe {
        assert_eq!(validateId(0), true);
        assert_eq!(validateId(maxDevices - 1), true);
    }
    log("validateId_checkCorrect1 end\n\0");
}

#[test_case]
fn validateId_checkIncorrect() {
    log("validateId_checkIncorrect start\n\0");
    unsafe {
        assert_eq!(validateId(maxDevices), false);
        assert_eq!(validateId(maxDevices + 1), false);
    }
    log("validateId_checkIncorrect end\n\0");
}
