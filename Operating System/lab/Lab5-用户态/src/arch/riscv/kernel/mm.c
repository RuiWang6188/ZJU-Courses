#include "mm.h"
#include "stdio.h"
extern char* _end;
struct phypage_link *phypage_list_top = NULL;

void kinit() {
  void* start = (void*)PAGE_UP(VA2PA(&_end));
  int count = 0;
  for(; (uint64)(start + PAGE_SIZE) <= PHY_END; start += PAGE_SIZE) {
	//printf("freeing %d page\n",count ++);
	kfree(start);
}
}

void kfree(void* page) {
  page = (void*)PAGE_DOWN(page);
  if((uint64)page < (uint64)VA2PA(&_end) || (uint64)page >= PHY_END)
  panic("Invalid physic page address to free");

  struct phypage_link *link;
  link = (struct phypage_link*)page;

  link->next = phypage_list_top;
  phypage_list_top = link;
}


void * kalloc(void) {
  struct phypage_link* link = phypage_list_top;
 // printf("kalloc : phypage_list_top:%d\n",phypage_list_top);
  if (link) {
    phypage_list_top = link->next;

    // Fill with 0xf
    memset((char*)link, 0xf, PAGE_SIZE);
  }

  else {
    panic("Run out of free physic pages");
  }

  return (void*)link;
}

void * alloc_page(void)
{
  struct phypage_link* link = (struct phypage_link *)PA2VA(phypage_list_top);
 // printf("alloc_page : phypage_list_top:%d\n",phypage_list_top);
 // printf("PA2VA(phypage_list_top):%d\n",PA2VA(phypage_list_top));
  if (link) {
    phypage_list_top = (struct phypage_link *)(link->next);

    // Fill with 0xf
    memset((char*)link, 0xf, PAGE_SIZE);
  }

  else {
    panic("Run out of free physic pages");
  }

 // puts("alloc_page() successful!\n");

  return (void*)link;
}
