#ifndef DEFAULT_H
#define DEFAULT_H

#include <types.h>

struct __attribute__((packed)) dos_4_0_ebpb_mediaDescriptor_t {
    bool doubleSided    :1;
    bool eightSector    :1;
    bool removable      :1;
    bool hardDisk       :1;
    uint_8 _reserved    :4;
};

struct __attribute__((packed, aligned(1))) dos_4_0_ebpb_t {
    uint_16 bytesPerLogicalSector;
    uint_8 logicalSectorsPerCluster;
    uint_16 reservedLogicalSectors;
    uint_8 numberOfFATs;
    uint_16 maxRootDirectoryEntries;
    uint_16 totalLogicalSectors;
    struct dos_4_0_ebpb_mediaDescriptor_t mediaDescriptor;
    uint_16 logicalSectorsPerFAT;
    uint_16 physicalSectorsPerTrack;
    uint_16 numberOfHeads;
    uint_32 hiddenSectors;
    uint_32 largeTotalLogicalSectors;
    uint_8 physicalDriveNumber;
    uint_8 flags;
    uint_8 extendedBootSignature;
    uint_32 volumeSerialNumber;
    uint_8 volumeLabel[11];
    uint_8 fileSystemType[8];
};
_Static_assert(sizeof(struct dos_4_0_ebpb_t) == 51, "Unexpected struct dos_4_0_ebpb_t size");

struct fat12_dir_attr_str_t {
    uint_8 _reserved : 1;
    uint_8 device : 1;
    uint_8 archive : 1;
    uint_8 subdirectory : 1;
    uint_8 volumeLabel : 1;
    uint_8 system : 1;
    uint_8 hidden : 1;
    uint_8 readOnly : 1;
};

union fat12_dir_attr_t {
    struct fat12_dir_attr_str_t model;
    uint_8 bytes;
};

struct __attribute__((packed, aligned(1))) fat12_dir_t {
    uint_8 filename[8];
    uint_8 extension[3];
    union fat12_dir_attr_t attributes;
    uint_16 _reserved;
    uint_16 creationTime;
    uint_16 creationDate;
    uint_16 lastAccessDate;
    uint_16 _fat12_ignored;
    uint_16 lastWriteTime;
    uint_16 lastWriteDate;
    uint_16 firstLogicalCluster;
    uint_32 fileSizeBytes;
};

#endif // DEFAULT_H