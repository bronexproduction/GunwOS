;
;  kernel_entry.s
;  GunwOS
;
;  Created by Artur Danielewski on 17.01.2018.
;

; This entry point is put at the beginning of the kernel by the linker
; If removed, there will be no call __kernel_start at the beginning of the kernel binary
; even though kernel.o contains such symbol
;
; entry point here must be at KERNEL_MEM_OFFSET, as defined in ../shared/data.s

BITS 32

EXTERN __kernel_start

    call __kernel_start
    cli
    hlt
    