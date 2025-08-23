# IO

## inb
```bash
static inline uint8_t inb(uint16_t port) {
    uint8_t value;
    __asm__ __volatile__ (
        "inb %1, %0" 
        :"=a"(value) 
        :"Nd"(port)
    );
    return value;
}
```
- inb = An x86 to read 1 byte (8-bit) from I/O port
- "=a"(value) = Put the read value into register AL and store in (value)
- "Nd"(port) = The compiler decides whether to encode the port as immediate (N) if port < 256 or in DX (d) if > 255
- retun value

## outb
```bash
static inline void outb(uint16_t port, uint8_t value) {
    __asm__ __volatile__ (
        "outb %0, %1"
        : 
        :"a"(value), "Nd"(port));
}
```
- outb = An x86 instruction that writes 1 byte (8-bit) to an I/O port
- "a"(value) = load (value) into AL
- "Nd"(port) = Put the port in immediate (N) or DX (d)

# PIC

## PIC Remap

### Keep the original IMR (Interrupt Mask Register) value
```bash
void pic_remap(int offset1, int offset2) {
    uint8_t master, slave;

    master = inb(0x21);
    slave = inb(0xA1);
}
```
- 0x21 = IMR of masterPIC
- 0xA1 = IMR of slavePIC
- Read and keep it → I'll restore it later because I don't want to change the original mask when I remap

### Send ICW1: Start Initialization
```bash
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
```
- 0x20 = Command register of master
- 0xA0 = Command register of slave
- 0x11 mean
    - bit0 = ICW4 needed
    - bit4 = Initialization

### Send ICW2: Tell IRQ Which offset start at 
```bash 
    outb(0x21, offset1);
    outb(0xA1, offset2);
```
- offset1 such as 0x20(irq0-7(32-39))
- offset2 such as 0x20(irq8-15(40-47))

### Send ICW3: Master/Slave wiring
```bash 
    outb(0x21, 0x02);
    outb(0xA1, 0x04);
```
- Slave is connected to IRQ2 of Master.
- Slave is connected to IRQ2.

### Send ICW4: Environment info
```bash 
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
```
- 0x01 = 8086/88 (MCS-80/85 mode disabled)

### Restore IMR (original mask)
```bash 
    outb(0x21, master);
    outb(0xA1, slave);
```
- Return the original mask value to prevent unwanted IRQs from being opened

# HOW USE 
```bash 
pic_remap(0x20, 0x28);
```
- irq0-7 to idt32-39
- irq8-15 to idt40-47