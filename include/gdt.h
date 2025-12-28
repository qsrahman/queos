#pragma once

#include <stdint.h>

#define NSEGS 6

// various segment selectors.
#define SEG_NULL 0   // NULL segment
#define SEG_KCODE 1  // kernel code segment
#define SEG_KDATA 2  // kernel data+stack segment
#define SEG_UCODE 3  // user code segment
#define SEG_UDATA 4  // user data+stack segment
#define SEG_TSS 5    // this process's task state segment

// This structure contains the value of one GDT entry.
// We use the attribute 'packed' to tell GCC not to change
// any of the alignment in the structure.
typedef struct gdt_entry {
    uint16_t limit_low;   // The lower 16 bits of the limit.
    uint16_t base_low;    // The lower 16 bits of the base.
    uint8_t base_middle;  // The next 8 bits of the base.
    uint8_t access;       // Access flags, determine what ring this segment can be
                          // used in.
    uint8_t granularity;
    uint8_t base_high;  // The last 8 bits of the base.
} __attribute__((packed)) gdt_entry_t;

// This struct describes a GDT pointer. It points to the start of
// our array of GDT entries, and is in the format required by the
// lgdt instruction.
typedef struct gdt_ptr {
    uint16_t limit;  // The upper 16 bits of all selector limits.
    uint32_t base;   // The address of the first gdt_entry_t struct.
} __attribute__((packed)) gdt_ptr_t;

// A struct describing a Task State Segment.
typedef struct tss_entry {
    uint32_t prev_tss;  // Previous TSS - if we used hardware task switching this would form a linked list.
    uintptr_t esp0;     // esp to load when we switching to kernel mode.
    uint32_t ss0;       // ss to load when we switching to kernel mode.
    uintptr_t esp1;     // Unused...
    uint32_t ss1;
    uintptr_t esp2;
    uint32_t ss2;
    uintptr_t cr3;
    uintptr_t eip;
    uint32_t eflags;
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uintptr_t esp;
    uintptr_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint32_t es;   // Value to load into ES when we change to kernel mode.
    uint32_t cs;   // Value to load into CS when we change to kernel mode.
    uint32_t ss;   // Value to load into SS when we change to kernel mode.
    uint32_t ds;   // Value to load into DS when we change to kernel mode.
    uint32_t fs;   // Value to load into FS when we change to kernel mode.
    uint32_t gs;   // Value to load into GS when we change to kernel mode.
    uint32_t ldt;  // Unused...
    uint16_t trap;
    uint16_t iomap_base;
} __attribute__((packed)) tss_entry_t;

static inline void lgdt(gdt_entry_t* p, int size) {
    volatile uint16_t gdt_desc[3];

    gdt_desc[0] = size - 1;
    gdt_desc[1] = (uint32_t)p;
    gdt_desc[2] = (uint32_t)p >> 16;

    __asm__ __volatile__("lgdt (%0)"
                         :
                         : "r"(gdt_desc));
}

void init_gdt(void);
void set_gdt_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);
