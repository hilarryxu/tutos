#ifndef TUTOS_ARCH_H_
#define TUTOS_ARCH_H_

#define PAGE_SIZE 4096

#define PAGE_ROUND_UP(sz) (((sz) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))
#define PAGE_ROUND_DOWN(a) (((a)) & ~(PAGE_SIZE - 1))

#endif  // TUTOS_ARCH_H_
