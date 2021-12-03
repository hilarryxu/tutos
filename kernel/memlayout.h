#ifndef TUTOS_MEMLAYOUT_H_
#define TUTOS_MEMLAYOUT_H_

// 物理内存大小（目前我们固定设置为 128MB）
#define DRAM_BASE 0x80000000L
#define PHYS_MEM_TOP (DRAM_BASE + (128 * 1024 * 1024))

#endif  // TUTOS_MEMLAYOUT_H_
