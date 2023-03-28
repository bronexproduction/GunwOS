//
//  gunwfilesys.h
//  GunwOS
//
//  Created by Artur Danielewski on 28.03.2023.
//

#ifndef GUNWOS_GUNWFILESYS_H
#define GUNWOS_GUNWFILESYS_H

/*
    File system descriptor

    In order to register a file system in kernel
    drivers must provide an instance of this type
*/
struct gnwFileSystemDescriptor {
    // /*
    //     Device type
    // */
    // enum gnwDeviceType type;

    // /*
    //     Hardware-specific API
    // */
    // struct gnwDeviceUHA api;

    // /*
    //     Device driver
    // */
    // struct gnwDeviceDriver driver;

    // /*
    //     Device friendly name
    // */
    // char *name;
};

#endif // GUNWOS_GUNWFILESYS_H
