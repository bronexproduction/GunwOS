//
//  stor.h
//  GunwOS
//
//  Created by Artur Danielewski on 02.04.2020.
//

#ifndef SYS_SERVICE_STOR_H
#define SYS_SERVICE_STOR_H

#include "../../driver/storage/storage.h"
#include "../../../include/gunwstor.h"

// /*
//     Storage volume type
// */
// enum s_stormgr_volType {
//     VTY_PHYSICAL
// };

/*
    Storage device
*/
enum s_stor_device {
    VDE_FLOPPY
};

/*
    Partition format (filesystem)
*/
enum s_stor_partitionFormat {
    GUNWFS_F
};

/*
    Partition descriptor
*/
struct s_stor_partitionDesc {
    enum s_stor_partitionFormat fileSystem;
};

/*
    Drive metadata
*/
struct s_stormgr_drvMeta {
    uint_8 present;    
};

/*
    Storage device descriptor
*/
struct s_stor_drvDescriptor {
    struct s_stormgr_drvMeta meta;
    enum s_stor_device device;
    uint_32 controllerDriverID;
    uint_8 controllerDriveIndex;
    struct gnwStorGeometry format;
};

/*
    Storage volume descriptor
*/
struct s_stor_volDescriptor {
    uint_32 driveID;
//     enum s_stormgr_volType type;
//     enum s_stormgr_device device;
//     struct s_stormgr_volMeta meta;
};

#endif // SYS_SERVICE_STOR_H
