#ifndef IDT_H
#define IDT_H
#include <stdint.h>

struct idt_entry {
    uint16_t offset_low;   // 16 บิตแรกของ address ของ ISR (Interrupt Service Routine)
    uint16_t selector;     // ค่า segment selector ของ code segment (CS) ที่ ISR อยู่
    uint8_t zero;          // บรรทัดนี้ต้องเป็น 0 เสมอ (reserved)
    uint8_t type_attr;     // กำหนดชนิดของ interrupt (type) และ attributes
    uint16_t offset_high;  // 16 บิตสูงของ address ของ ISR
} __attribute__((packed)); 
// __attribute__((packed)) บังคับให้ compiler ไม่เติม padding byte ใดๆ เพื่อให้ layout ตรงตาม hardware

// โครงสร้างสำหรับ pointer ของ IDT (สำหรับใช้คำสั่ง lidt)
struct idt_ptr {
    uint16_t limit;  // ขนาดของ IDT - 1
    uint32_t base;   // Address ของ IDT ใน memory
} __attribute__((packed)); 

void init_idt(void);

#endif