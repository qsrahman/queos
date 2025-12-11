#pragma once

#include <types.h>

static inline void cli(void) {
    __asm__ __volatile__("cli");
}

static inline void sti(void) {
    __asm__ __volatile__("sti");
}

static inline uint read_eflags(void) {
    uint eflags;
    __asm__ __volatile__("pushfl; popl %0"
                         : "=r"(eflags));
    return eflags;
}

static inline void write_eflags(uint eflags) {
    __asm__ __volatile__("pushl %0; popfl"
                         :
                         : "r"(eflags));
}

static inline uchar inb(ushort port) {
    uchar data;

    __asm__ __volatile__("in %1,%0"
                         : "=a"(data)
                         : "d"(port));
    return data;
}

static inline ushort inw(ushort port) {
    ushort data;
    __asm__ __volatile__("inw %1, %0"
                         : "=a"(data)
                         : "d"(port));
    return data;
}

static inline void insl(int port, void* addr, int cnt) {
    __asm__ __volatile__("cld; rep insl"
                         : "=D"(addr), "=c"(cnt)
                         : "d"(port), "0"(addr), "1"(cnt)
                         : "memory", "cc");
}

static inline void outb(ushort port, uchar data) {
    __asm__ __volatile__("out %0,%1"
                         :
                         : "a"(data), "d"(port));
}

static inline void outw(ushort port, ushort data) {
    __asm__ __volatile__("out %0,%1"
                         :
                         : "a"(data), "d"(port));
}

static inline void outsl(int port, const void* addr, int cnt) {
    __asm__ __volatile__("cld; rep outsl"
                         : "=S"(addr), "=c"(cnt)
                         : "d"(port), "0"(addr), "1"(cnt)
                         : "cc");
}

static inline void stosb(void* addr, int data, int cnt) {
    __asm__ __volatile__("cld; rep stosb"
                         : "=D"(addr), "=c"(cnt)
                         : "0"(addr), "1"(cnt), "a"(data)
                         : "memory", "cc");
}

static inline void stosl(void* addr, int data, int cnt) {
    __asm__ __volatile__("cld; rep stosl"
                         : "=D"(addr), "=c"(cnt)
                         : "0"(addr), "1"(cnt), "a"(data)
                         : "memory", "cc");
}

static inline void ltr(ushort sel) {
    __asm__ __volatile__("ltr %0"
                         :
                         : "r"(sel));
}

static inline uint xchg(volatile uint* addr, uint newval) {
    uint result;

    // The + in "+m" denotes a read-modify-write operand.
    __asm__ __volatile__("lock; xchgl %0, %1"
                         : "+m"(*addr), "=a"(result)
                         : "1"(newval)
                         : "cc");
    return result;
}

static inline void lcr3(uint val) {
    __asm__ __volatile__("movl %0,%%cr3"
                         :
                         : "r"(val));
}

static inline void
cpuid(uint info, uint* eaxp, uint* ebxp, uint* ecxp, uint* edxp) {
    uint eax, ebx, ecx, edx;

    __asm__ __volatile__("cpuid"
                         : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                         : "a"(info));
    if (eaxp)
        *eaxp = eax;
    if (ebxp)
        *ebxp = ebx;
    if (ecxp)
        *ecxp = ecx;
    if (edxp)
        *edxp = edx;
}
