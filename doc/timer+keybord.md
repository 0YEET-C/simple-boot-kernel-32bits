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