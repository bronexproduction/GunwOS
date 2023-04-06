// #include <stdio.h>
// #include <stdlib.h>
// #include <stdint.h>
// #include <string.h>

// #define SECTOR_BYTES 512

// #define DOS_4_0_EBPB_BYTES 51
// #define FAT12_DIR_BYTES 32
// #define FAT12_BYTES_PER_ENTRY 12
// #define FAT12_RESERVED_ENTRY_COUNT 2

// const char * const nullErr = "NULL data passed - exiting";

// size_t readSegment(uint8_t *dst, size_t offset, size_t byteCount, FILE *src);
// int checkFATInfo(const struct dos_4_0_ebpb_t * const bpb);
// int checkRootDirInfo(const struct dos_4_0_ebpb_t * const bpb);
// const uint8_t *const *const createFATsArray(const struct dos_4_0_ebpb_t * const bpb, FILE * const src, size_t *offset);
// void readAndPrintFile(FILE * const src,
//                       const uint8_t * const * const fats,
//                       const struct fat12_dir_t * const rootDir,
//                       const struct dos_4_0_ebpb_t * const bpb,
//                       const char * const filename);
// uint32_t clusterAlignedBytes(const uint32_t bytes, const struct dos_4_0_ebpb_t * const bpb);
// uint32_t clusterSize(const struct dos_4_0_ebpb_t * const bpb);

// uint8_t fat12_empty(const struct fat12_dir_t * const dir);
// uint8_t fat12_longFileName(const struct fat12_dir_t * const dir);
// const struct fat12_dir_t * fat12_find(const struct fat12_dir_t * const rootDir,
//                                       const struct dos_4_0_ebpb_t * const bpb,
//                                       const char * const filename);
// uint32_t fat12_readFile(FILE * const src,
//                         const uint8_t * const * const fats,
//                         const struct dos_4_0_ebpb_t * const bpb, 
//                         const struct fat12_dir_t * const dir, 
//                         uint8_t *buf, 
//                         uint8_t *err);
// uint32_t fat12_read(FILE * const src,
//                     uint8_t *dst,
//                     uint16_t cluster,
//                     const struct dos_4_0_ebpb_t * const bpb,
//                     uint8_t *fat12err);
// uint16_t fat12_nextCluster(const uint8_t * const * const fats,
//                            const struct dos_4_0_ebpb_t * const bpb,
//                            uint16_t cluster,
//                            uint8_t *fatDataEqual);
// uint16_t fat12_entry(const uint8_t * const fat, const struct dos_4_0_ebpb_t * const bpb, uint16_t cluster);

// int main(int argc, char *argv[]) {

//     /*
//         Read boot sector
//     */
//     struct bootSector_t bootSector;
//     if (!(bytesRead = readSegment((uint8_t *)&bootSector, offset, SECTOR_BYTES, bin))) {
//         printf("Error reading bootsector bytes\n");
//         fclose(bin);
//         return 1;
//     }
//     offset += bytesRead;

//     if (!checkFATInfo(&bootSector.bpb)) {
//         fclose(bin);
//         exit(1);
//     }
//     if (!checkRootDirInfo(&bootSector.bpb)) {
//         fclose(bin);
//         exit(1);
//     }

//     /*
//         Read FAT tables section
//     */
//     const uint8_t *const *const fats = createFATsArray(&bootSector.bpb, bin, &offset);
//     if (!fats) {
//         printf("Error reading FAT table bytes\n");
//         fclose(bin);
//         return 1;
//     }

//     /*
//         Read root directory section
//     */
//     struct fat12_dir_t *rootDir = calloc(bootSector.bpb.maxRootDirectoryEntries, sizeof(struct fat12_dir_t));
//     if (!(bytesRead = readSegment((uint8_t *)rootDir, offset, bootSector.bpb.maxRootDirectoryEntries * sizeof(struct fat12_dir_t), bin))) {
//         printf("Error reading root directory bytes\n");
//         fclose(bin);
//         return 1;
//     }

//     /*
//         Read the specified file (if any)
//     */
//     if (fileToBeReadName) {
//         readAndPrintFile(bin, fats, rootDir, &bootSector.bpb, fileToBeReadName);
//     }

//     fclose(bin);
//     return 0;
// }

// size_t readSegment(uint8_t *dst, size_t offset, size_t byteCount, FILE *src) {
//     if (!dst || !src) {
//         printf("%s\n", nullErr);
//         exit(3);
//     }
//     fseek(src, offset, SEEK_SET);
//     size_t bytes = fread(dst, 1, byteCount, src);

//     return bytes == byteCount ? bytes : 0;
// }

// const uint8_t *const *const createFATsArray(const struct dos_4_0_ebpb_t * const bpb, FILE * const src, size_t *offset) {
//     uint8_t **fats = malloc(sizeof(uint8_t *) * bpb->numberOfFATs);
//     size_t bytesRead;
        
//     size_t size = bpb->bytesPerLogicalSector;
//     size_t count = bpb->logicalSectorsPerFAT;

//     for (int i = 0; i < bpb->numberOfFATs; ++i) {
//         fats[i] = calloc(size, count);
//         if (!(bytesRead = readSegment(fats[i], *offset, size * count, src))) {
//             return NULL;
//         }
//         *offset += bytesRead;
//     }

//     return (const uint8_t *const *const)fats;
// }

// uint8_t fat12_empty(const struct fat12_dir_t * const dir) {
//     if (!dir) {
//         return 1;
//     }
//     uint8_t *dirBytes = (uint8_t *)dir;
//     for (size_t i = 0; i < sizeof(struct fat12_dir_t); ++i) {
//         if (dirBytes[i]) break;
//         if (i == sizeof(struct fat12_dir_t) - 1) {
//             return 1;
//         }
//     }

//     return 0;
// }

// uint32_t fat12_readFile(FILE * const src,
//                         const uint8_t * const * const fats,
//                         const struct dos_4_0_ebpb_t * const bpb, 
//                         const struct fat12_dir_t * const dir, 
//                         uint8_t *buf, 
//                         uint8_t *err) {
//     *err = 0;
//     if (dir->firstLogicalCluster < 2) {
//         *err = 1;
//         return 0;
//     }

//     uint32_t totalBytesRead = 0;
//     uint16_t cluster = dir->firstLogicalCluster;
//     uint32_t clusterSizeBytes = clusterSize(bpb);
//     uint8_t fatDataEqual;
//     uint8_t fat12err;
//     do {
//         if (totalBytesRead >= dir->fileSizeBytes) {
//             printf("FAT integrity failure - expected EOF.\n");
//             *err = 2;
//             break;
//         }
//         if (!fat12_isValidForRead(cluster)) {
//             printf("Invalid cluster number: u\n", cluster);
//             *err = 3;
//             break;
//         }
//         uint16_t nextCluster = fat12_nextCluster(fats, bpb, cluster, &fatDataEqual);
//         if (!fatDataEqual) {
//             printf("FAT integrity failure - FATs data not equal.\n");
//             *err = 4;
//             break;
//         }
//         if (!fat12_isValidForRead(nextCluster)) {
//             printf("FAT entry for cluster %u is invalid\n", cluster);
//             *err = 5;
//             break;
//         }
//         printf("Reading cluster %u\n", cluster);
//         uint32_t bytesRead = fat12_read(src, buf + totalBytesRead, cluster, bpb, &fat12err);
//         if (bytesRead != clusterSizeBytes || fat12err) {
//             printf("Error reading cluster.\n");
//             *err = 5;
//             break;
//         }

//         totalBytesRead += bytesRead;
        
//         printf("Next cluster: %x\n", nextCluster);
//         cluster = nextCluster;
//     } while (!fat12_isEOF(cluster));

//     return totalBytesRead;
// }

// uint32_t fat12_read(FILE * const src,
//                     uint8_t *dst,
//                     uint16_t cluster,
//                     const struct dos_4_0_ebpb_t * const bpb,
//                     uint8_t *fat12err) {
    
//     printf("FAT12: Reading cluster %u\n", cluster);

//     size_t bytesPerCluster = clusterSize(bpb);
//     size_t fatClusterCount = bpb->logicalSectorsPerFAT / bpb->logicalSectorsPerCluster;
//     size_t rootDirClusterCount = bpb->maxRootDirectoryEntries * FAT12_DIR_BYTES / bytesPerCluster;
//     size_t firstClusterOffset = 1 + fatClusterCount * bpb->numberOfFATs + rootDirClusterCount;
//     size_t clusterOffset = firstClusterOffset + cluster - FAT12_RESERVED_ENTRY_COUNT;
//     printf("Logical cluster offset: %lu\n", clusterOffset);

//     return readSegment(dst,
//                        clusterOffset * bytesPerCluster,
//                        bytesPerCluster,
//                        src);
// }
