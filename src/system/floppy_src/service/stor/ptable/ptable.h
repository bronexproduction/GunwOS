//
//  ptable.h
//  GunwOS
//
//  Created by Artur Danielewski on 28.01.2021.
//

#include "../../../../../lib/stdgunw/types.h"
#include "../stor.h"

/*
    Partition table definition
*/
struct s_stor_ptable {
    /*
        Detects partition table on given drive
    */
    uint_8 (*detect)(const struct s_stor_drvDescriptor);

    /*
        Returns number of partitions on the drive

        Assumes it uses correct partition table
    */
   uint_32 (*count)(const struct s_stor_drvDescriptor);

    /*
        Returns partition descriptor at given index

        Assumes it uses correct partition table
    */
    struct s_stor_partitionDesc (*partition)(const struct s_stor_drvDescriptor, const uint_32 partitionIndex);
};
