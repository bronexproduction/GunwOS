;
;  fat12err.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 18.08.2022.
;

BITS 16

fat12_err_entryNotFound:

    mov bx, MSG_FAT12_ENTRY_NOT_FOUND
    jmp fat12_err_print

fat12_err_sizeLimitExceeded:

    mov bx, MSG_FAT12_SIZE_LIMIT_EXCEEDED
    jmp fat12_err_print

fat12_err_sizeInvalid:

    mov bx, MSG_FAT12_SIZE_INVALID
    jmp fat12_err_print

fat12_err_fatValidationFailed:

    mov bx, MSG_FAT12_FAT_VALIDATION_FAILED
    jmp fat12_err_print

fat12_err_clusterNotValidForRead:

    mov bx, MSG_FAT12_CLUSTER_NOT_VALID_FOR_READ
    jmp fat12_err_print

fat12_err_fatEntryInvalid:

    mov bx, MSG_FAT12_FAT_ENTRY_INVALID
    jmp fat12_err_print

fat12_err_fatEntryInvalidEOFExpected:

    mov bx, MSG_FAT12_FAT_ENTRY_INVALID_EOF_EXPECTED

fat12_err_print:

    call print_str_16
    cli
    hlt
