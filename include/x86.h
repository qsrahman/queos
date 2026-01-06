#pragma once

#include <stdint.h>

static inline void cli(void) {
    __asm__ __volatile__("cli");
}

static inline void sti(void) {
    __asm__ __volatile__("sti");
}

static inline uint32_t read_eflags(void) {
    uint32_t eflags;
    __asm__ __volatile__("pushfl; popl %0"
                         : "=r"(eflags));
    return eflags;
}

static inline void write_eflags(uint32_t eflags) {
    __asm__ __volatile__("pushl %0; popfl"
                         :
                         : "r"(eflags));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t data;

    __asm__ __volatile__("in %1,%0"
                         : "=a"(data)
                         : "d"(port));
    return data;
}

static inline uint16_t inw(uint16_t port) {
    uint16_t data;
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

static inline void outb(uint16_t port, uint8_t data) {
    __asm__ __volatile__("out %0,%1"
                         :
                         : "a"(data), "d"(port));
}

static inline void outw(uint16_t port, uint16_t data) {
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

static inline void ltr(uint16_t sel) {
    __asm__ __volatile__("ltr %0"
                         :
                         : "r"(sel));
}

static inline uint32_t xchg(volatile uint32_t* addr, uint32_t newval) {
    uint32_t result;

    // The + in "+m" denotes a read-modify-write operand.
    __asm__ __volatile__("lock; xchgl %0, %1"
                         : "+m"(*addr), "=a"(result)
                         : "1"(newval)
                         : "cc");
    return result;
}

static inline void lcr3(uint32_t val) {
    __asm__ __volatile__("movl %0,%%cr3"
                         :
                         : "r"(val));
}

static inline void
cpuid(uint32_t info, uint32_t* eaxp, uint32_t* ebxp, uint32_t* ecxp, uint32_t* edxp) {
    uint32_t eax, ebx, ecx, edx;

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
