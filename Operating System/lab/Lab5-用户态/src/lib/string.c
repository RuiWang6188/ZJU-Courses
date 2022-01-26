#include "string.h"

void* memset(void *dst, int c, uint n) {
  char *cdst = (char *) dst;
  for(int i = 0; i < n; i++){
    cdst[i] = c;
  }
  return dst;
}


void *
memmove (void *dest, const void *src, size_t len)
{
  char *d = dest;
  const char *s = src;
  if (d < s)
    while (len--)
      *d++ = *s++;
  else
    {
      char *lasts = s + (len-1);
      char *lastd = d + (len-1);
      while (len--)
        *lastd-- = *lasts--;
    }
  return dest;
}

void* memcpy(void* dst, const void* src, size_t len)
{
	if (NULL == dst || NULL == src) {
		return NULL;
	}

	void* ret = dst;

	if (dst <= src || (char*)dst >= (char*)src + len) {
		//没有内存重叠，从低地址开始复制
		while (len--) {
			*(char*)dst = *(char*)src;
			dst = (char*)dst + 1;
			src = (char*)src + 1;
		}
	}
	else {
		//有内存重叠，从高地址开始复制
		src = (char*)src + len - 1;
		dst = (char*)dst + len - 1;
		while (len--) {
			*(char*)dst = *(char*)src;
			dst = (char*)dst - 1;
			src = (char*)src - 1;
		}
	}
	return ret;
}
