//
//  help.rs
//  GunwOS
//
//  Created by Artur Danielewski on 09.01.2023.
//

#![no_std]

use core::ffi::c_char;

extern "C" {
    fn user_cli_puts(szText: *const c_char);
}

#[no_mangle]
fn cmd_help(_params: &[char]) {
    let message = "Command not recognized: help - did you mean \"halp\"?\0";
    unsafe {
        user_cli_puts(message.as_ptr() as *const c_char);
    }
}
