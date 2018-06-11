section .bss
align 16
stack_bottom:
resb 16384
stack_top:

section .text
global _start:function (_start.end - _start)
_start:
    mov esp, stack_top
    ; setup GDT, Paging, etc.
    extern kernel_main
    call kernel_main

cli
.hang
hlt
jmp .hang

.end: