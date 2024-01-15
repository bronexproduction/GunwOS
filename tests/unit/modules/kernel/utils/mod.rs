use core::ffi::c_char;
use kernel_symbols::k_log_log;

extern "C" {
    
    pub static KERNEL_PANIC_FLAG: bool;

    /*
        bus
    */
    pub fn bus_outb(port: u16, val: u8);

    /*
        cpu
    */
    pub fn cpu_cli();
}

pub fn log(msg: &str) {
    unsafe {
        k_log_log(msg.as_ptr() as *const c_char);
    }
}
