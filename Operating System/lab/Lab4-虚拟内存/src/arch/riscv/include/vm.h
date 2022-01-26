
#define PGSIZE 4096 // bytes per page
#define PGSHIFT 12  // bits of offset within a page

#define PGROUNDUP(sz)  (((sz)+PGSIZE-1) & ~(PGSIZE-1))
#define PGROUNDDOWN(a) (((a)) & ~(PGSIZE-1))

#define PTE_V (1L << 0) // valid
#define PTE_R (1L << 1)
#define PTE_W (1L << 2)
#define PTE_X (1L << 3)
#define PTE_U (1L << 4) // 1 -> user can access

// shift a physical address to the right place for a PTE.
#define PA2PTE(pa) ((((uint64)pa) >> 12) << 10)

#define PTE2PA(pte) (((pte) >> 10) << 12)

#define PTE_FLAGS(pte) ((pte) & 0x3FF)

// extract the three 9-bit page table indices from a virtual address.
#define PXMASK          0x1FF // 9 bits
#define PXSHIFT(level)  (PGSHIFT+(9*(level)))
#define PX(level, va) ((((uint64) (va)) >> PXSHIFT(level)) & PXMASK)

// one beyond the highest possible virtual address.
// MAXVA is actually one bit less than the max allowed by
// Sv39, to avoid having to sign-extend virtual addresses
// that have the high bit set.
#define MAXVA (1L << (9 + 9 + 9 + 12 - 1))

typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int  uint32;
typedef unsigned long uint64;


typedef uint64 pte_t;
typedef uint64 *pagetable_t; // 512 PTEs
typedef uint64 pde_t;

// memory layout
#define UARTBASE 0x10000000L
#define KERNBASE 0x80000000L
#define HIGHBASE 0xffffffe000000000L

#define KERNSIZE (16 * 1024 * 1024) //16MB for kernel memroy
#define PHYSTOP (KERNBASE + 16*1024*1024)

// functions declaration
void* memset(void *dst, int c, uint n);
void * kalloc(void);
void kfree(void *pa);
void freerange(void *pa_start, void *pa_end);
void kinit();
pte_t *walk(pagetable_t pagetable, uint64 va, int alloc);
int create_mapping(uint64 *pgtbl, uint64 va, uint64 pa, uint64 sz, int perm);
void paging_init(void);
