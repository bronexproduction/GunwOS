//
//  help.rs
//  GunwOS
//
//  Created by Artur Danielewski on 09.01.2023.
//

#![no_std]

extern "C" { fn user_cli_puts(szText: &str); }

#[no_mangle]
fn cmd_help(_params: &[char]) {
    unsafe {
        user_cli_puts("Command not recognized: help - did you mean \"halp\"?");
    }
}
