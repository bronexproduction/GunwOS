;
;  fat12err.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 18.08.2022.
;

BITS 16

fat12_err_entryNotFound:

    mov bx, FAT12_LOADER_NOT_FOUND_STRING
    jmp fat12_err_print

fat12_err_sizeLimitExceeded:
fat12_err_sizeInvalid:

    mov bx, FAT12_FILE_SIZE_INVALID
    jmp fat12_err_print

fat12_err_fatValidationFailed:
fat12_err_clusterNotValidForRead:
fat12_err_fatEntryInvalid:
fat12_err_fatEntryInvalidEOFExpected:

    mov bx, FAT12_INVALID_ERROR_STRING

fat12_err_print:

    call print_err_16
