#pragma once

#include <types.h>

#define NSEGS 6

// various segment selectors.
#define SEG_KCODE 1  // kernel code
#define SEG_KDATA 2  // kernel data+stack
#define SEG_UCODE 3  // user code
#define SEG_UDATA 4  // user data+stack
#define SEG_TSS 5    // this process's task state

// This structure contains the value of one GDT entry.
// We use the attribute 'packed' to tell GCC not to change
// any of the alignment in the structure.
typedef struct gdt_entry {
    ushort limit_low;   // The lower 16 bits of the limit.
    ushort base_low;    // The lower 16 bits of the base.
    uchar base_middle;  // The next 8 bits of the base.
    uchar access;       // Access flags, determine what ring this segment can be
                        // used in.
    uchar granularity;
    uchar base_high;  // The last 8 bits of the base.
} __attribute__((packed)) gdt_entry_t;

// This struct describes a GDT pointer. It points to the start of
// our array of GDT entries, and is in the format required by the
// lgdt instruction.
typedef struct gdt_ptr {
    ushort limit;  // The upper 16 bits of all selector limits.
    uint base;     // The address of the first gdt_entry_t struct.
} __attribute__((packed)) gdt_ptr_t;

// A struct describing a Task State Segment.
typedef struct tss_entry {
    uint prev_tss;  // Previous TSS - if we used hardware task switching this would form a linked list.
    uint esp0;      // esp to load when we switching to kernel mode.
    uint ss0;       // ss to load when we switching to kernel mode.
    uint esp1;      // Unused...
    uint ss1;
    uint esp2;
    uint ss2;
    uint cr3;
    uint eip;
    uint eflags;
    uint eax;
    uint ecx;
    uint edx;
    uint ebx;
    uint esp;
    uint ebp;
    uint esi;
    uint edi;
    uint es;   // Value to load into ES when we change to kernel mode.
    uint cs;   // Value to load into CS when we change to kernel mode.
    uint ss;   // Value to load into SS when we change to kernel mode.
    uint ds;   // Value to load into DS when we change to kernel mode.
    uint fs;   // Value to load into FS when we change to kernel mode.
    uint gs;   // Value to load into GS when we change to kernel mode.
    uint ldt;  // Unused...
    ushort trap;
    ushort iomap_base;
} __attribute__((packed)) tss_entry_t;

static inline void lgdt(gdt_entry_t* p, int size) {
    volatile ushort gdt_desc[3];

    gdt_desc[0] = size - 1;
    gdt_desc[1] = (uint)p;
    gdt_desc[2] = (uint)p >> 16;

    __asm__ __volatile__("lgdt (%0)"
                         :
                         : "r"(gdt_desc));
}

void init_gdt(void);
void set_gdt_gate(int num, uint base, uint limit, uchar access, uchar gran);
