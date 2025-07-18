;[BITS 32]
bits 32

%macro ISR_NOERR 1
	global _isr%1
	_isr%1:
		cli
		push byte 0
		push byte %1
		jmp isr_common_stub
%endmacro

%macro ISR_ERR 1
	global _isr%1
	_isr%1:
		cli
		push byte %1
		jmp isr_common_stub
%endmacro

%macro IRQ_ENTRY 2
	global _irq%1
	_irq%1:
		cli
		push byte 0
		push byte %2
		jmp irq_common_stub
%endmacro

MBALIGN equ 1 << 0
MBINFO  equ 1 << 1
MBFLAGS equ MBALIGN | MBINFO
MAGIC  equ 0x1BADB002
CHECKSUM equ -(MAGIC + MBFLAGS)

align 4
    dd    MAGIC
    dd    MBFLAGS
    dd    CHECKSUM

section .bss
align 16
stack_bottom:
    resb 16384
stack_top:


section .text

global _start
extern main_kernel

ISR_NOERR 0
ISR_NOERR 1
ISR_NOERR 2
ISR_NOERR 3
ISR_NOERR 4
ISR_NOERR 5
ISR_NOERR 6
ISR_NOERR 7
ISR_ERR   8
ISR_NOERR 9
ISR_ERR   10
ISR_ERR   11
ISR_ERR   12
ISR_ERR   13
ISR_ERR   14
ISR_NOERR 15
ISR_NOERR 16
ISR_NOERR 17
ISR_NOERR 18
ISR_NOERR 19
ISR_NOERR 20
ISR_NOERR 21
ISR_NOERR 22
ISR_NOERR 23
ISR_NOERR 24
ISR_NOERR 25
ISR_NOERR 26
ISR_NOERR 27
ISR_NOERR 28
ISR_NOERR 29
ISR_NOERR 30
ISR_NOERR 31

IRQ_ENTRY 0, 32
IRQ_ENTRY 1, 33
IRQ_ENTRY 2, 34
IRQ_ENTRY 3, 35
IRQ_ENTRY 4, 36
IRQ_ENTRY 5, 37
IRQ_ENTRY 6, 38
IRQ_ENTRY 7, 39
IRQ_ENTRY 8, 40
IRQ_ENTRY 9, 41
IRQ_ENTRY 10, 42
IRQ_ENTRY 11, 43
IRQ_ENTRY 12, 44
IRQ_ENTRY 13, 45
IRQ_ENTRY 14, 46
IRQ_ENTRY 15, 47

global idt_load
extern idtp

idt_load:
    lidt [idtp]
    ret


extern fault_handler
isr_common_stub:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10   ; Load the Kernel Data Segment descriptor!
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp   ; Push us the stack
    push eax
    mov eax, fault_handler
    call eax       ; A special call, preserves the 'eip' register
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8     ; Cleans up the pushed error code and pushed ISR number
    iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!

extern irq_handler
irq_common_stub:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp
    push eax
    mov eax, irq_handler
    call eax
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    iret


global check_cpuid
check_cpuid:
    pushfd
    pushfd
    xor dword [esp],0x00200000
    popfd
    pushfd
    pop eax
    xor eax,[esp]
    popfd
    and eax, 0x00200000
    ret


_start:
		lgdt[gdt]
		mov ax, 0x10
		mov ds, ax
		mov es, ax
		mov fs, ax
		mov gs, ax
		mov ss, ax
		jmp 0x08:pmain


extern start_kernel
extern end_kernel

pmain:
    mov esp, stack_top

    push ebx
    push eax
    push end_kernel
    push start_kernel

    call main_kernel

_stop:
    cli
    hlt
    jmp _stop

section .data
; basic flat model
gdt_base:
		dq 0
.gdt_text:
		dw 0xffff
		dw 0
		db 0
		db 0b10011010
		db 0b11001111
		db 0

.gdt_data:
		dw 0xffff
		dw 0
		db 0
		db 0b10010010
		db 0b11001111
		db 0
gdt_end:

gdt:
		dw gdt_end - gdt_base - 1
		dd gdt_base
