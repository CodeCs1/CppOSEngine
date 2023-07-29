.section .text

.extern handle

.macro HandleException num
.global _ZN9Interrupt16handleException\num\()Ev
_ZN9Interrupt16handleException\num\()Ev:
    movb $\num, (num)
    jmp int_bottom
.endm

.macro HandleRequest num
.global _ZN9Interrupt17handlerequest\num\()Ev

_ZN9Interrupt17handlerequest\num\()Ev:
    movb $\num + 0x20, (num)
    pushl $0
    jmp int_bottom
.endm

HandleRequest 0x00
HandleRequest 0x01

int_bottom:
    pusha
    pushl %ebp
    pushl %edi
    pushl %esi

    pushl %edx
    pushl %ecx
    pushl %ebx
    pushl %eax


    pushl %esp
    push (num)
    call handle
    movl %eax, %esp

    popl %eax
    popl %ebx
    popl %ecx
    popl %edx

    popl %esi
    popl %edi
    popl %ebp
    popa

    add $4, %esp

.global _ZN9Interrupt13IgnorerequestEv

_ZN9Interrupt13IgnorerequestEv:
    iret
.data 
    num: .byte 0
