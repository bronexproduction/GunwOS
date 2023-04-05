//
//  help.rs
//  GunwOS
//
//  Created by Artur Danielewski on 09.01.2023.
//

#![no_std]
extern crate alloc;

use core::ffi::c_char;
use alloc::string::String;
use alloc::vec::Vec;

extern "C" {
    fn user_cli_puts(szText: *const c_char);
}

#[no_mangle]
fn cmd_help(_params: &[char]) {
    let message = CStr::from("Command not recognized: help - did you mean \"halp\"?");
    unsafe {
        user_cli_puts(message.as_ptr());
    }
}


struct CStr {
    buffer: Vec<u8>
}

impl CStr {
    pub fn from(string: &str) -> Self {
        let mut bytes = String::from(string).into_bytes();
        bytes.push(0);
        Self {
            buffer: bytes,
        }
    }

    pub fn as_ptr(&self) -> *const c_char {
        self.buffer.as_ptr() as *const c_char
    }
}