#ifndef DEFAULT_H
#define DEFAULT_H

#include <stdint.h>

struct __attribute__((packed, aligned(1))) dos_4_0_ebpb_t {
    uint16_t bytesPerLogicalSector;
    uint8_t logicalSectorsPerCluster;
    uint16_t reservedLogicalSectors;
    uint8_t numberOfFATs;
    uint16_t maxRootDirectoryEntries;
    uint16_t totalLogicalSectors;
    uint8_t mediaDescriptor;
    uint16_t logicalSectorsPerFAT;
    uint16_t physicalSectorsPerTrack;
    uint16_t numberOfHeads;
    uint32_t hiddenSectors;
    uint32_t largeTotalLogicalSectors;
    uint8_t physicalDriveNumber;
    uint8_t flags;
    uint8_t extendedBootSignature;
    uint32_t volumeSerialNumber;
    uint8_t volumeLabel[11];
    uint8_t fileSystemType[8];
};

struct __attribute__((packed, aligned(1))) bootSector_t {
    uint8_t _reserved_1[3];
    uint8_t oemName[8];
    struct dos_4_0_ebpb_t bpb;
    uint8_t _reserved_2[450];
};

struct fat12_dir_attr_str_t {
    uint8_t _reserved : 1;
    uint8_t device : 1;
    uint8_t archive : 1;
    uint8_t subdirectory : 1;
    uint8_t volumeLabel : 1;
    uint8_t system : 1;
    uint8_t hidden : 1;
    uint8_t readOnly : 1;
};

union fat12_dir_attr_t {
    struct fat12_dir_attr_str_t model;
    uint8_t bytes;
};

struct __attribute__((packed, aligned(1))) fat12_dir_t {
    uint8_t filename[8];
    uint8_t extension[3];
    union fat12_dir_attr_t attributes;
    uint16_t _reserved;
    uint16_t creationTime;
    uint16_t creationDate;
    uint16_t lastAccessDate;
    uint16_t _fat12_ignored;
    uint16_t lastWriteTime;
    uint16_t lastWriteDate;
    uint16_t firstLogicalCluster;
    uint32_t fileSizeBytes;
};

struct bootSector_t defaultBootSector = {
    "\0\0\0",
    "MSDOS5.0",
        {
        (uint16_t) 512,
        (uint8_t) 1,
        (uint16_t) 1,
        (uint8_t) 2,
        (uint16_t) 224,
        (uint16_t) 2880,
        (uint8_t) 0xF0,
        (uint16_t) 9,
        (uint16_t) 18,
        (uint16_t) 2,
        (uint32_t) 0,
        (uint32_t) 0,
        (uint8_t) 0,
        (uint8_t) 0x0,
        (uint8_t) 0x29,
        (uint32_t) 1850637505,
        "NO NAME    ",
        "FAT12   "
    },
    "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
};

#endif // DEFAULT_H