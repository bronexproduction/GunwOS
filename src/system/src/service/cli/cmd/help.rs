//
//  help.rs
//  GunwOS
//
//  Created by Artur Danielewski on 09.01.2023.
//

#![no_std]

extern "C" { pub fn s_trm_puts(szText: &str); }

#[no_mangle]
fn cmd_help(_params: &[char]) {
    unsafe {
        s_trm_puts("Command not recognized: help - did you mean \"halp\"?");
    }
}
