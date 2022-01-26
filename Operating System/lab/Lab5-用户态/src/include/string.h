#pragma once

#include "types.h"
#include "stddef.h" // size_t

void* memset(void *dst, int c, uint n);

void* memmove(void* dest, const void* src, size_t count);
