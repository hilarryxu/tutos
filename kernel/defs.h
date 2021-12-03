#ifndef TUTOS_DEFS_H_
#define TUTOS_DEFS_H_

#include "types.h"

// printf.c
void printf(char *fmt, ...);
void panic(char *s);

// string.c
void *memset(void *str, int c, size_t n);
#define bzero(p, sz) memset((p), 0, (sz))

// pmm.c
int pmm_initialize(void);
phys_addr_t pmm_alloc(void);
void pmm_free(phys_addr_t pa);
void pmm_test(void);

#endif  // TUTOS_DEFS_H_
