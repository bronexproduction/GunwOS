//
//  system.c
//  GunwOS
//
//  Created by Artur Danielewski on 13.01.2023.
//

/*
    Initialization of high-level system modules
*/

extern void s_cli_init(void);

void s_init() {
    s_cli_init();
}
