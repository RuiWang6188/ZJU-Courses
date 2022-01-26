#pragma once

#include "string.h"
#include "stdio.h"
#include "types.h"
#include "rinux.h"

struct phypage_link {
  struct phypage_link *next;
};

void*   kalloc(void);
void    kfree(void *);
void    kinit(void);
void*   alloc_page(void);