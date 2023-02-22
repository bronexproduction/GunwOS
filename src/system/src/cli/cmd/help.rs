//
//  help.rs
//  GunwOS
//
//  Created by Artur Danielewski on 09.01.2023.
//

#![no_std]

extern "C" {
    fn c_trm_puts(szText: *const c_char);
}

#[no_mangle]
fn cmd_help(_params: &[char]) {
    let message = "Command not recognized: help - did you mean \"halp\"?";
    let message_null_terminated = CString::new(message).unwrap();
    unsafe {
        c_trm_puts(message_null_terminated.as_ptr());
    }
}
