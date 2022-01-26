#pragma once

#include "rinux.h"
#include "types.h"
#include "string.h"


void kvminit();
void kvmmap(pagetable_t pgtbl, uint64 va, uint64 pa, uint64 sz, int perm);
int mappages(pagetable_t pagetable, uint64 va, uint64 size, uint64 pa, int perm);
pte_t* walk(pagetable_t pagetable, uint64 va, int alloc);
void uvminit(pagetable_t pagetable);
pagetable_t init_task_pagetable(void);
pagetable_t uvmcreate(void);


