//
//  filsys.h
//  GunwOS
//
//  Created by Artur Danielewski on 28.01.2021.
//

#include "../../../../../lib/stdgunw/types.h"
#include "../stor.h"

/*
    Partition table definition
*/
struct s_stor_filsys {
    /*
        Detects file system on given drive
    */
    uint_8 (*detect)(const struct s_stor_drvDescriptor);

    /*
        Returns partition descriptor
    */
    struct s_stor_partitionDesc (*partition)(const struct s_stor_drvDescriptor);
};
