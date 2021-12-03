#ifndef TUTOS_MEMLAYOUT_H_
#define TUTOS_MEMLAYOUT_H_

#define PAGE_SIZE 4096

#define PAGE_ROUND_UP(sz) (((sz) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))
#define PAGE_ROUND_DOWN(a) (((a)) & ~(PAGE_SIZE - 1))

// 物理内存大小（目前我们固定设置为 128MB）
#define DRAM_BASE 0x80000000L
#define PHYS_MEM_TOP (DRAM_BASE + (128 * 1024 * 1024))

#endif  // TUTOS_MEMLAYOUT_H_
