//
//  gunwuha_drivectrl_desc.h
//  Universal Hardware Abstraction for a storage device
//  GunwOS
//
//  Created by Artur Danielewski on 09.03.2023.
//

#ifndef GUNWOS_GUNWUHA_DRIVECTRL_DESC_H
#define GUNWOS_GUNWUHA_DRIVECTRL_DESC_H

#include <types.h>

struct gnwDeviceUHA_driveCtrl_desc {
    /*
        Total number of supported drives
    */
    uint_8 driveCount;

    /*
        Controller removability flag

        E.g. flash drives
    */
    bool removable;

    /*
        Drive media removability flag

        E.g. floppy disks, memory cards
    */
    bool removableMedia;
};

#endif // GUNWOS_GUNWUHA_DRIVECTRL_DESC_H
