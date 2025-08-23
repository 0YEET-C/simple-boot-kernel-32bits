# Timer
- timer is a irq0(32)

```bash
static uint32_t timer_frequency = 18;

if (int_no == 32) {
        static uint32_t ticks = 0;
        ticks++;

        if(ticks % timer_frequency == 0) {
            print("1 second passed\n");
        }
    }

void timer_init(uint32_t frequency) {
    timer_frequency = frequency;

    uint32_t divisor = 1193180 / frequency;

    outb(0x43, 0x36);
    outb(0x40, divisor & 0xFF);
    outb(0x40, divisor >> 8);
}
```
- check Interrupt = 32 
- PIT work in clock 1.193180 MHz 
- outb(0x43, 0x36); -> PIT Command Register to (0x43)
- 0x36 = channel 0, access low+high byte, mode 3 (square wave), binary
- outb(0x40, divisor & 0xFF); -> low byte to 0x40 and outb(0x40, divisor >> 8); -> high byte to 0x40
- irq0 trigger According to the set frequency

# Keyborad

## Keyboard Handler
```bash 
global irq1
irq1:
    cli
    push dword 0
    push dword 33
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

    call keyboard_handler

    pop gs
    pop fs
    pop es
    pop ds
    popa

    add esp, 8
    iretd
```
- irq1(33)
- call keyboard handler in c

## Map for keyborad
```bash
unsigned char scancode_to_ascii[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
    'a','s','d','f','g','h','j','k','l',';','\'','`',   0,
    '\\','z','x','c','v','b','n','m',',','.','/',   0,
    '*', 0,' ',
};
```

## keyboard handler c
```bash
static int shift_pressed = 0;

void keyboard_handler() {
    unsigned char scancode = inb(0x60);

    if (scancode == 0x2A || scancode == 0x36) shift_pressed = 1;
    if (scancode == 0xAA || scancode == 0xB6) shift_pressed = 0;

    if (!(scancode & 0x80) && scancode < 128) {
        char c = scancode_to_ascii[scancode];

        if (c) {
            if (shift_pressed && c >= 'a' && c <= 'z') {
                c = c - 'a' + 'A';
            }
            print_char(c);
        }
    }

    outb(0x20, 0x20);
}
```
- any time press/release keyborad send scancode to port (0x60)
- (0x2A) = Left Shift(press) and (0x36) = Right Shift(press)
- (0xAA) = Left Shift(release) and (0xB6) = Right Shift(release)
- 1  = uppercase and 0 = lowercase