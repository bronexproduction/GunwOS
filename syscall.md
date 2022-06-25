# GunwOS: System Call

The GunwOS provides a simple API for potential apps written for this OS.
Software interrupt 0x69 is used to perform syscall in x86 architecture.

## Example

Below there is the example in assembler (Intel syntax).

```asm
mov 0x01, eax   ; Select "rdb" function (see the table below)
mov 0x34, ebx   ; Select 0x34 bus address
int 69h         ; Perform syscall
; in eax register there is value at 0x34 bus address
```

## Table

Below there is a table that describes all public system calls.

name | number (eax) | ebx | ecx | output (eax) | description
---- | ------------ | --- | --- | ------------ | -----------
rdb | 0x01 | bus address | | value at bus address |
wrb | 0x02 | bus address | value | |
exit | 0x03 | return code | | |
printl | 0x04 | char array pointer | length | printed characters count | if length == 0 - text is read until reaches '\0'
heap_allocate | 0x05 | size | | adress | Allocating dynamic memory on the heap
heap_free | 0x06 | adress | length | | Free previously allocated memory on the heap

## Appendix A

For more information see
 - [System / Kernel / Syscall / func.h](src/system/src/kernel/syscall/func.h)
 - [System / Kernel / Syscall / syscall.c](src/system/src/kernel/syscall/syscall.c)
