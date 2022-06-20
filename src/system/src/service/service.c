//
//  service.c
//  GunwOS
//
//  Created by Artur Danielewski on 21.01.2021.
//

extern void s_trm_init();
extern void s_devmgr_init();
extern void s_stormgr_init();
extern void s_cli_init();

void s_svc_init() {
    s_trm_init();
    s_devmgr_init();
    s_stormgr_init();
    s_cli_init();
}
