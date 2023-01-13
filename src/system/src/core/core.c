//
//  core.c
//  GunwOS
//
//  Created by Artur Danielewski on 13.01.2023.
//

/*
    Initialization of system core
*/

extern void c_drv_loadDefaults();
#warning Terminal to be implemented as character device
extern void c_trm_init();
extern void s_init();

void c_init() {
    c_drv_loadDefaults();
    c_trm_init();
    s_init();
}
