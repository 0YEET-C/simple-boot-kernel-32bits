#include "include/idt.h"
#include <stdint.h>


struct idt_entry idt[256];  // ประกาศ IDT มี 256 entry (x86 รองรับ interrupt vector 0-255)
struct idt_ptr idtp;    // ตัว pointer ของ IDT สำหรับโหลดไปที่ CPU ด้วย lidt   

// ฟังก์ชันสำหรับตั้งค่า entry ใน IDT
void set_idt_gate(uint8_t num, uint32_t handler, uint16_t sel, uint8_t flags) {
    idt[num].offset_low = handler & 0xFFFF;  // บิตต่ำ 16 บิตของ address ของ ISR  
    idt[num].selector = sel;  // เลือก code segment (CS) ที่ ISR จะรัน                     )
    idt[num].zero = 0;  // ต้องเป็น 0 เสมอ (reserved                       
    idt[num].type_attr = flags;  // กำหนด type และ attributes เช่น interrupt gate, present               
    idt[num].offset_high = (handler >> 16) & 0xFFFF;  // บิตสูง 16 บิตของ address ของ ISR
}


extern void idt_flush(uint32_t);  // ฟังก์ชันที่ถูก implement ด้วย assembly สำหรับโหลด IDT ไปที่ CPU
extern uint32_t (*isr_table[32]);  // ตาราง ISR สำหรับ 32 interrupt ของ CPU (0-31)
extern uint32_t (*irq_table[16]);  // ตาราง IRQ ของ hardware (0-15) 

// ฟังก์ชันสำหรับ initialize IDT ทั้งหมด
void init_idt(void) {
    idtp.limit = sizeof(idt) - 1;      // ขนาด IDT - 1
    idtp.base = (uint32_t)idt;         // address ของ IDT ใน memory

    // เริ่มเซ็ตค่า IDT ทั้งหมดเป็น default (0, CS=0x08, type_attr=0x8E)
    // ป้องกันค่า garbage และให้ทุก vector present เป็น interrupt gate
    for (int i = 0; i < 256; i++) {
        set_idt_gate(i, 0, 0x08, 0x8E); 
    }
    // เซ็ต entry ของ CPU exceptions (interrupt vector 0-31)
    for (int v = 0; v < 32; v++) {
        set_idt_gate(v, (uint32_t)isr_table[v], 0x08, 0x8E);
    }
    // เซ็ต entry ของ hardware IRQ (interrupt vector 32-47)
    for (int q = 0; q < 16; q++) {
        set_idt_gate(32 + q, (uint32_t)irq_table[q], 0x08, 0x8E);
    }
    // โหลด IDT ไปที่ CPU (คำสั่ง lidt ใน assembly)
    idt_flush((uint32_t)&idtp);
}