;
;  a20_32.s
;  GunwOS Bootloader
;  Part of GunwOS project
;
;  Created by Artur Danielewski on 20.01.2018.
;

BITS 32

    ; ---------------------------------------
    ; Checking access to extended memory
    ;
    ; Checks if the memory wraps over 1MB limit to ensure access to extended memory
    ; ---------------------------------------

a20_32_check:
    pushad

    ; ---------------------------------------
    ; Set retry count to zero
    ; (only single retry allowed)
    ; ---------------------------------------

    mov ecx, 0

    ; ---------------------------------------
    ; Prepare source and destination
    ; memory locations
    ;
    ; EAX (ESI) - conventional memory offset
    ; EBX (EDI) - extended memory address
    ;             (ESI + 1MiB)
    ; ---------------------------------------

    mov eax, A20_CHECK_OFFSET
    add ebx, A20_CHECK_OFFSET + 0x100000

    ; ---------------------------------------
    ; Set source memory address
    ; ---------------------------------------

    mov esi, eax

a20_32_check_overlap:
    
    ; ---------------------------------------
    ; Reset destination memory address
    ; to default value
    ; ---------------------------------------
    
    mov edi, ebx

    ; ---------------------------------------
    ; Backup original source memory content,
    ; get and increment the value found
    ; under destination address,
    ; put it under source address
    ; and then compare the two locations
    ; ---------------------------------------

    mov edx, [esi]

    mov ebx, [edi]
    inc ebx
    mov [esi], ebx

    cmpsd

    ; ---------------------------------------
    ; Restore original memory content
    ;
    ; NOTE: CMPSD alters ESI and EDI
    ;       and therefore previous values
    ;       have to be stored  
    ;       or the operation reverse to 
    ;       the CMPSD operation mode
    ;       has to be applied
    ;       (INC/DEC - depends on DF flag)
    ; ---------------------------------------

    mov esi, eax
    mov [esi], edx

    ; ---------------------------------------
    ; If values are not equal - A20 set
    ; ---------------------------------------

    jne a20_32_ok

a20_32_enable:

    ; ---------------------------------------
    ; If values are equal - set A20 and retry
    ; ---------------------------------------

    ; ---------------------------------------
    ; Check if already retried
    ; and end with error if it did
    ; ---------------------------------------

    cmp ecx, 0
    jnz a20_32_error

    ; ---------------------------------------
    ; TODO
    ; ACTIVATE A20
    ; ---------------------------------------

    inc ecx
    jmp a20_32_check_overlap

a20_32_error:
    mov ebx, MSG_A20_DISABLED_ERROR
    call print_str_32
    
    cli
    hlt

a20_32_ok:
    mov ebx, MSG_A20_ENABLED
    call print_str_32

    popad
    