#include <gdt.h>
#include <string.h>

static gdt_entry_t gdt[NSEGS];

void set_gdt_gate(int num, uint base, uint limit, uchar access, uchar gran) {
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16) & 0x0F;

    gdt[num].granularity |= gran & 0xF0;
    gdt[num].access = access;
}

void init_gdt(void) {
    // Null segment
    set_gdt_gate(0, 0, 0, 0, 0);
    // P DPL DT Type -> x xx x xxxx
    // Code segment (DPL=0) 0x9A = 1 00 1 1010 ->
    // execute/read G D 0 0 xxxx -> 0xCF = 1 1 0 0 1111
    set_gdt_gate(SEG_KCODE, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    // Data segment (DPL=0) 0x92 = 1 00 1 0010 -> read/write
    set_gdt_gate(SEG_KDATA, 0, 0xFFFFFFFF, 0x92, 0xCF);
    // User mode (DPL=3) code segment 0xFA = 1 11 1 1010
    set_gdt_gate(SEG_UCODE, 0, 0xFFFFFFFF, 0xFA, 0xCF);
    // User mode (DPL=3) data segment 0xFA = 1 11 1 0010
    set_gdt_gate(SEG_UDATA, 0, 0xFFFFFFFF, 0xF2, 0xCF);

    // load gdt
    lgdt(gdt, sizeof(gdt));
}
