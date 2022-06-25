;
;  preloader_data.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 08.03.2020.
;

; This data structure should correspond with struct k_pld_bootData (preloader_data.h)

preloader_data:
preloader_data_bootDrive:
    db 0 ; filled with DL register value at boot start
