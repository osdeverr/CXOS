global cx_idt_impl_load_anchor
extern cx_idt_anchor
cx_idt_impl_load_anchor:
lidt [cx_idt_anchor]
ret

extern cx_idt_binding_isr_handler
extern cx_idt_binding_irq_handler

global cx_idt_impl_irq_handler
global cx_idt_impl_isr_handler

cx_idt_impl_irq_handler:
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
mov eax, cx_idt_binding_irq_handler
call eax
pop eax
pop gs
pop fs
pop es
pop ds
popa
add esp, 8
iret

; use the correct stubs to handle error codes and push dummies!

; This is our common ISR stub. It saves the processor state, sets
; up for kernel mode segments, calls the C-level fault handler,
; and finally restores the stack frame.
cx_idt_impl_isr_handler:
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
mov eax, cx_idt_binding_isr_handler
call eax       ; A special call, preserves the 'eip' register
pop eax
pop gs
pop fs
pop es
pop ds
popa
add esp, 8     ; Cleans up the pushed error code and pushed ISR number
iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!

