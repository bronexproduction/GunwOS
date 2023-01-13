//
//  help.rs
//  GunwOS
//
//  Created by Artur Danielewski on 09.01.2023.
//

#![no_std]

extern "C" { fn k_trm_puts(szText: &str); }

#[no_mangle]
fn cmd_help(_params: &[char]) {
    unsafe {
        k_trm_puts("Command not recognized: help - did you mean \"halp\"?");
    }
}
