//
//  help.rs
//  GunwOS
//
//  Created by Artur Danielewski on 09.01.2023.
//

#![no_std]

fn cmd_help(params: [char]) {
    s_trm_puts("Command not recognized: help - did you mean \"halp\"?");
}
