## IDT Stuct
```bash 
struct idt_entry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t type_attr;
    uint16_t offset_high;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct idt_entry idt[256];
struct idt_ptr idtp;

```
- idt_entry for set gate for Interrupt
- set idt to 0-255 or 256 gate for Interrupt
- idt_ptr for loade struct in asm 

## IDT Gate setup
```bash 
void set_idt_gate(uint8_t num, uint32_t handler, uint16_t sel, uint8_t flags) {
    idt[num].offset_low = handler & 0xFFFF;
    idt[num].selector = sel;
    idt[num].zero = 0;
    idt[num].type_attr = flags;
    idt[num].offset_high = (handler >> 16) & 0xFFFF;
}
```
- offset low -> handler and offest high -> handler >> 16
- sel -> code_seg and flags -> flags all this in gdt 

## IDT INIT
```bash 
extern void idt_flush(uint32_t);

void init_idt(void) {
    idtp.limit = sizeof(idt) - 1;
    idtp.base = (uint32_t)idt;

    idt_flush((uint32_t)&idtp);
```
- idt_flush is idt loader in asm
- limit -> sizof idt struct - 1
- base -> target
### idt_flush
```bash 
[bits 32]
global idt_flush
idt_flush:
    mov eax, [esp + 4]
    lidt [eax]
    ret
```
- mov esp +4 it reverse use eax for collect data
- use lidt [eax] for load idt to ram
- ret to return to c after loade idt

# ISR 
afer set gate idt finish

# FLOW
CPU → IDT → ISR → C handler → return

## Set Gate For ISR
```bash 
extern uint32_t (*isr_table[32]);

for (int v = 0; v < 32; v++) {
        set_idt_gate(v, (uint32_t)isr_table[v], 0x08, 0x8E);
    }
```
- isr0-31 all 32
- use isr_table in asm 
### ISR Table
```bash 
global isr_table

isr_table:
    dd isr0, isr1 , isr2 , isr3 , isr4 , isr5 , isr6 , isr7
    dd isr8, isr9 , isr10 , isr11 , isr12 , isr13 , isr14 , isr15
    dd isr16, isr17 , isr18 , isr19 , isr20 , isr21 , isr22 , isr23
    dd isr24, isr25 , isr26 , isr27 , isr28 , isr29 , isr30 , isr31
```
- have isr0-31 for loop

## ISR Handler
```bash
extern isr_handler

%macro ISR_NOERR 1
global isr%1
isr%1:
    cli
    push dword 0 
    push dword %1
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
    call isr_handler
    add esp, 4

    pop gs
    pop fs
    pop es
    pop ds
    popa

    add esp, 8
    sti
    iretd
%endmacro

%macro ISR_ERR 1
global isr%1
isr%1:
    cli
    push dword %1
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
    call isr_handler
    add esp, 4

    pop gs
    pop fs
    pop es
    pop ds
    popa

    add esp, 4
    sti
    iretd
%endmacro
```
- isr err 8, 10, 11, 12, 13, 14, 17 (Has Error Code)
- isr noerr 0–7, 10–13, 16, , 18-32 (No Error Code)
- 0x8E or type_attr to choice err or noerr

## ISR C Handler
```bash 
void isr_handler(regs_t *r) {
    print("Interrupt: ");
    print_int(r->int_no);
    print("\n");
}
```
- print Interrupt number
### ISR Struct
```bash 
typedef struct regs {
    uint32_t gs, fs, es, ds; 
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code; 
    uint32_t eip, cs, eflags, useresp, ss;   
} regs_t;
```
- collect information from push

# IRQ

# FLOW
CPU → IDT → IRQ → C handler → return

## Set Gate for IQR
```bash
extern uint32_t (*irq_table[16]);

for (int q = 0; q < 16; q++) {
        set_idt_gate(32 + q, (uint32_t)irq_table[q], 0x08, 0x8E);
    }
```
### IQR_Table
```bash
global irq_table

irq_table:
    dd irq0, irq1, irq2, irq3, irq4, irq5, irq6, irq7
    dd irq8, irq9, irq10, irq11, irq12, irq13, irq14, irq15
```
- irq0-15

## IRQ Handler
```bash
%macro IRQ 2
global irq%1
irq%1:
    cli
    push dword 0
    push dword %2
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

    push %2
    call irq_handler
    add esp, 4

    pop gs
    pop fs
    pop es
    pop ds
    popa

    add esp, 8
    sti
    iretd
%endmacro
```
- push regis 
- call iqr hadler

## IQR C Handler
```bash
void irq_handler(uint32_t int_no) {
    if (int_no != 32) {
        print("IRQ: ");
        print_int(int_no);
        print("\n");
    }

    if (int_no >= 40) {
        outb(0xA0, 0x20);
    }
    outb(0x20, 0x20);
```
- if != 32 because irq32 for timer
- if int_no > 40 irq 8-15(40-47) to slave(0xA0) + master(0x20)
- irq0-7(32-39) ot master(0x20)
