#include "types.h"

void *
memset(void *dst, int c, size_t n)
{
  size_t i;
  char *cdst = (char *)dst;

  for (i = 0; i < n; i++) {
    cdst[i] = c;
  }

  return dst;
}
