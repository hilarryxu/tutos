#ifndef TUTOS_DEFS_H_
#define TUTOS_DEFS_H_

#include "types.h"
#include "macros.h"

// printf.c
void printf(char *fmt, ...);
void panic(char *s);

// debug.c
void dump_stacktrace(void);

#define BUG_ON(x)                                                     \
  do {                                                                \
    if (unlikely(x)) {                                                \
      printf("Bug in %s() at %s:%d\n", __func__, __FILE__, __LINE__); \
      dump_stacktrace();                                              \
      panic("Please reset the system ...\n");                         \
    }                                                                 \
  } while (0)

#define BUG() BUG_ON(1)

#define WARN_ON(x)                                             \
  ({                                                           \
    if (unlikely(x)) {                                         \
      printf("%s() at %s:%d\n", __func__, __FILE__, __LINE__); \
      dump_stacktrace();                                       \
    }                                                          \
    (x);                                                       \
  })

#define WARN(x, msg...)                                        \
  ({                                                           \
    if (unlikely(x)) {                                         \
      printf(msg);                                             \
      printf("%s() at %s:%d\n", __func__, __FILE__, __LINE__); \
      dump_stacktrace();                                       \
    }                                                          \
    (x);                                                       \
  })

// string.c
void *memset(void *str, int c, size_t n);
#define bzero(p, sz) memset((p), 0, (sz))

// pmm.c
int pmm_initialize(void);
phys_addr_t pmm_alloc(void);
void pmm_free(phys_addr_t pa);
void pmm_test(void);

#endif  // TUTOS_DEFS_H_
