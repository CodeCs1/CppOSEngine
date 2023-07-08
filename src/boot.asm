bits 32
section .text
    align 4
    dd    0x1badb002
    dd    0x00
    dd    -(0x1badb002+0x00)

global start
extern main_kernel

start:
    mov ax,0x4F02
    mov bx,0x4118
    call main_kernel

_stop:
    cli
    hlt
    jmp _stop