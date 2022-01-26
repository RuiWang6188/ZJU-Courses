#include "vm.h"
#include "put.h"

extern char text_end[];
extern char rodata_start[];
extern char rodata_end[];
extern char data_start[];
extern char data_end[];
extern char bss_start[];
extern char _end[];


// data structure definition
uint64 *kernel_pgtbl;

struct run {
  struct run *next;
};

struct {
  //struct spinlock lock;
  struct run *freelist;
} kmem;


void paging_init(void)
{
	kinit();

	kernel_pgtbl = (uint64*)kalloc();
	//memset(kernel_pgtbl, 0, PGSIZE);

	/****** 1. equal value mapping  ******/
	create_mapping(kernel_pgtbl, UARTBASE, UARTBASE, PGSIZE, PTE_R | PTE_W);    												// UART direct mapping
	create_mapping(kernel_pgtbl, KERNBASE, KERNBASE, (uint64)text_end - KERNBASE, PTE_R | PTE_X | PTE_W);								// text direct mapping		
	create_mapping(kernel_pgtbl, (uint64)rodata_start, (uint64)rodata_start, (uint64)rodata_end - (uint64)rodata_start, PTE_R);				// rodata direct mapping
	create_mapping(kernel_pgtbl, (uint64)data_start, (uint64)data_start, (uint64)data_end - (uint64)data_start, PTE_R | PTE_W);	//data direct mapping
	create_mapping(kernel_pgtbl, (uint64)bss_start, (uint64)bss_start, KERNBASE + KERNSIZE - (uint64)bss_start, PTE_R | PTE_W);	// bss to end of 16MB direct mapping

	/****** 2. kernel mapping to high mem: 0xffffffe000000000  ******/
	uint64 offset = (uint64)HIGHBASE - (uint64)KERNBASE;
  create_mapping(kernel_pgtbl, KERNBASE + offset, KERNBASE, (uint64)text_end - KERNBASE, PTE_R | PTE_X | PTE_W);               // text direct mapping    
  create_mapping(kernel_pgtbl, (uint64)rodata_start + offset, (uint64)rodata_start, (uint64)rodata_end - (uint64)rodata_start, PTE_R);       // rodata direct mapping
  create_mapping(kernel_pgtbl, (uint64)data_start + offset, (uint64)data_start, (uint64)data_end - (uint64)data_start, PTE_R | PTE_W); //data direct mapping
  create_mapping(kernel_pgtbl, (uint64)bss_start + offset, (uint64)bss_start, KERNBASE + KERNSIZE - (uint64)bss_start, PTE_R | PTE_W); // bss to end of 16MB direct mapping


}

int create_mapping(uint64 *pgtbl, uint64 va, uint64 pa, uint64 sz, int perm)
{
  uint64 a, last;
  pte_t *pte;

  a = PGROUNDDOWN(va);
  last = PGROUNDDOWN(va + sz - 1);
  for(;;){
    if((pte = walk(pgtbl, a, 1)) == 0)
      return -1;
    if(*pte & PTE_V)
      puts("remap!\n");
    *pte = PA2PTE(pa) | perm | PTE_V;
    if(a == last)
      break;
    a += PGSIZE;
    pa += PGSIZE;
  }
  return 0;
}

pte_t *walk(pagetable_t pagetable, uint64 va, int alloc)
{
  //if(va >= MAXVA)
  //  puts("walk error!\n");

  for(int level = 2; level > 0; level--) {
    pte_t *pte = &pagetable[PX(level, va)];
    if(*pte & PTE_V) {
      pagetable = (pagetable_t)PTE2PA(*pte);
    } else {
      if(!alloc || (pagetable = (pde_t*)kalloc()) == 0)
        return 0;
      //memset(pagetable, 0, PGSIZE);
      *pte = PA2PTE(pagetable) | PTE_V;
    }
  }
  return &pagetable[PX(0, va)];
}



void kinit()
{
  //initlock(&kmem.lock, "kmem");
  freerange(_end, (void*)PHYSTOP);
}

void freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    kfree(p);
}

// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void kfree(void *pa)
{
  struct run *r;

  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < _end || (uint64)pa >= PHYSTOP)
    puts("kfree error!\n");

  // Fill with junk to catch dangling refs.
  //memset(pa, 1, PGSIZE);

  r = (struct run*)pa;

  //acquire(&kmem.lock);
  r->next = kmem.freelist;
  kmem.freelist = r;
  //release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void * kalloc(void)
{
  struct run *r;

  //acquire(&kmem.lock);
  r = kmem.freelist;
  if(r)
    kmem.freelist = r->next;
  //release(&kmem.lock);

  //if(r)
    //memset((char*)r, 5, PGSIZE); // fill with junk
  return (void*)r;
}

void* memset(void *dst, int c, uint n)
{
  char *cdst = (char *) dst;
  int i;
  for(i = 0; i < n; i++){
    cdst[i] = c;
  }
  return dst;
}
  
