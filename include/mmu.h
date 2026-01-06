#pragma once

#include <stdint.h>

// Memory layout
#define EXTMEM 0x00100000    // Start of extended memory
#define PHYSTOP 0x0E000000   // 224 MB Top physical memory
#define DEVSPACE 0xFE000000  // Other devices are at high addresses

// Key addresses for address space layout (see kmap in vm.c for layout)
#define KERNBASE 0xC0000000           // First kernel virtual address
#define KERNLINK (KERNBASE + EXTMEM)  // Address where kernel is linked

#define V2P(a) (((uint32_t)(a)) - KERNBASE)
#define P2V(a) ((void*)(((char*)(a)) + KERNBASE))

// Page table/directory entry flags.
#define PTE_P 0x001   // Present
#define PTE_W 0x002   // Writeable
#define PTE_U 0x004   // User
#define PTE_PS 0x080  // Page Size

// Page directory and page table constants.
#define PGSIZE 4096  // bytes mapped by a page
#define PGSHIFT 12   // log2(PGSIZE)
#define PTXSHIFT 12  // offset of PTX in a linear address
#define PDXSHIFT 22  // offset of PDX in a linear address

#define NPDENTRIES 1024  // # directory entries per page directory
#define NPTENTRIES 1024  // # PTEs per page table

#define PGROUNDUP(sz) (((sz) + PGSIZE - 1) & ~(PGSIZE - 1))
#define PGROUNDDOWN(a) (((a)) & ~(PGSIZE - 1))

// A virtual address 'va' has a three-part structure as follows:
//
// +--------10------+-------10-------+---------12----------+
// | Page Directory |   Page Table   | Offset within Page  |
// |      Index     |      Index     |                     |
// +----------------+----------------+---------------------+
//  \--- PDX(va) --/ \--- PTX(va) --/

// page directory index
#define PDX(va) (((uintptr_t)(va) >> PDXSHIFT) & 0x3FF)

// page table index
#define PTX(va) (((uintptr_t)(va) >> PTXSHIFT) & 0x3FF)

// Address in page table or page directory entry
#define PTE_ADDR(pte) ((uintptr_t)(pte) & ~0xFFF)
#define PTE_FLAGS(pte) ((uint32_t)(pte) & 0xFFF)

// construct virtual address from indexes and offset
#define PGADDR(d, t, o) ((void*)((d) << PDXSHIFT | (t) << PTXSHIFT | (o)))

typedef uint32_t pte_t;
typedef uint32_t pde_t;

static inline void switch_pgdir(uint32_t pg_dir) {
    __asm__ __volatile__("movl %0, %%cr3" ::"r"(pg_dir));
}
