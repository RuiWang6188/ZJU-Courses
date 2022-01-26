#include "vm.h"
#include "mm.h"
#include "device.h"
#include "symbol.h"
#include "string.h"
#include "stdio.h"

pagetable_t kpgtbl; 


void kvminit() {

  char * tempsscratch = kalloc();
  write_csr(sscratch,PA2VA(tempsscratch + PAGE_SIZE));

  kpgtbl = (pagetable_t) kalloc();
  memset(kpgtbl, 0, PAGE_SIZE);
    
  // map devices
  uint64 uart = PA2VA(get_device_addr(UART_MMIO));
  kvmmap(kpgtbl, uart, VA2PA(uart), get_device_size(UART_MMIO), (PTE_R | PTE_W ));
  uint64 poweroff = PA2VA(get_device_addr(POWEROFF_MMIO));
  kvmmap(kpgtbl, poweroff, VA2PA(poweroff), get_device_size(POWEROFF_MMIO), (PTE_R | PTE_W ));
  // map kernel text executable and read-only.
  kvmmap(kpgtbl, (uint64)&text_start, VA2PA((uint64)&text_start), (uint64)&text_end - (uint64)&text_start, (PTE_R | PTE_X  | PTE_W));
  // map kernel data and the physical RAM we'll make use of.
  kvmmap(kpgtbl, (uint64)&rodata_start, VA2PA((uint64)&rodata_start), (uint64)&rodata_end - (uint64)&rodata_start, (PTE_R | PTE_W));
  kvmmap(kpgtbl, (uint64)&data_start, VA2PA((uint64)&data_start), (uint64)PHY_END - VA2PA((uint64)&data_start), (PTE_R | PTE_W | PTE_X));

 // map kernel stacks

}

// add a mapping to the kernel page table.
void kvmmap(pagetable_t kpgtbl, uint64 va, uint64 pa, uint64 sz, int perm)
{
  if(mappages(kpgtbl, va, sz, pa, perm) != 0)
    panic("kvmmap");
}

// Return the address of the PTE in page table pagetable
// that corresponds to virtual address va.  If alloc!=0,
// create any required page-table pages.
pte_t * walk(pagetable_t pagetable, uint64 va, int alloc)
{

  for(int level = 2; level > 0; level--) {
    pte_t *pte = &pagetable[PX(level, va)];
    if(*pte & PTE_V) {
      pagetable = (pagetable_t)PTE2PA(*pte);
    } else {
      if(!alloc || (pagetable = (pagetable_t)kalloc()) == 0) // kalloc
        return 0;
      memset(pagetable, 0, PAGE_SIZE);
      *pte = PA2PTE(pagetable) | PTE_V;
    }
  }
  return &pagetable[PX(0, va)];
}

// Create PTEs for virtual addresses starting at va that refer to
// physical addresses starting at pa. va and size might not
// be page-aligned. Returns 0 on success, -1 if walk() couldn't
// allocate a needed page-table page.
int mappages(pagetable_t pagetable, uint64 va, uint64 size, uint64 pa, int perm)
{
  uint64 a, last;
  pte_t *pte;
//  printf("mappages():map begin...\n");
  
  a = PAGE_DOWN(va);
  last = PAGE_DOWN(va + size - 1);
  
  for(;;){
//  printf("mappages(): begin = %4x\n",a);
//  printf("mappages(): last = %4x\n",last);
    if((pte = walk(pagetable, a, PAGE_ALLOC)) == 0){
	panic("walk() cannot allocate needed page-table page");
	return -1;	
    }
//	printf("mappages():walk-PASS\n");
    if(*pte & PTE_V)
      panic("remap");
    *pte = PA2PTE(pa) | perm | PTE_V;
    if(a == last)
      break;
    a += PAGE_SIZE;
    pa += PAGE_SIZE;
  }
//  printf("mappages():map done!\n");
  return 0;
}


void uvminit(pagetable_t pagetable)
{
  char* mem = 0x84000000;
  mappages(pagetable, 0, PAGE_SIZE, (uint64)mem, PTE_W|PTE_R|PTE_X|PTE_U);

}


pagetable_t init_task_pagetable(void)
{
  pagetable_t pagetable;

  // An empty page table.
  pagetable = uvmcreate();
  if(pagetable == 0)
    return 0;

  return pagetable;
}

pagetable_t uvmcreate(void)
{
  pagetable_t pagetable;
  pagetable = (pagetable_t) kalloc();
//  pagetable = (pagetable_t)alloc_page();
  if(pagetable == 0)
    return 0;
  memset(pagetable, 0, PAGE_SIZE);
  return pagetable;
}

