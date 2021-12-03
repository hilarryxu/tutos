#include "defs.h"
#include "arch.h"
#include "memlayout.h"

// 空闲页框头部
struct pmm_block {
  struct pmm_block *next;
};

// 物理内存分配器
struct pmm_allocator {
  // 空闲页框链表
  struct pmm_block *freelist;
};

// kernel.ld 中内核结束的地址
extern char _kernel_end[];

// 物理内存分配器
static struct pmm_allocator pmm_allocator = {.freelist = NULL};

//---------------------------------------------------------------------
// 释放 [pa_start, pa_end) 的物理内存
//---------------------------------------------------------------------
static void
free_range(phys_addr_t pa_start, phys_addr_t pa_end)
{
  char *p;

  p = (char *)PAGE_ROUND_UP(pa_start);  // 按页对齐一下起始地址
  for (; p + PAGE_SIZE <= (char *)pa_end; p += PAGE_SIZE)
    pmm_free((phys_addr_t)p);
}

//---------------------------------------------------------------------
// 初始化: 物理内存管理
//---------------------------------------------------------------------
int
pmm_initialize(void)
{
  printf("[pmm] initialize\n");

  free_range((phys_addr_t)_kernel_end, PHYS_MEM_TOP);

  return 0;
}

//---------------------------------------------------------------------
// 申请分配一个物理页框
//---------------------------------------------------------------------
phys_addr_t
pmm_alloc(void)
{
  struct pmm_block *block;

  // 从空闲链表头部取出一块
  block = pmm_allocator.freelist;
  if (block) {
    pmm_allocator.freelist = block->next;
    return (phys_addr_t)block;
  }

  return 0;
}

//---------------------------------------------------------------------
// 释放物理地址对应的页框
//---------------------------------------------------------------------
void
pmm_free(phys_addr_t pa)
{
  // 检查地址是否按页对齐，以及区间是否合法
  // [page_aligned(_kernel_end), PHYS_MEM_TOP)
  if ((pa & 0xFFF) || (char *)pa < _kernel_end ||
      (unsigned long)pa >= PHYS_MEM_TOP)
    panic("pmm_free");

  // 插入空闲链表头部
  struct pmm_block *block = (struct pmm_block *)pa;
  block->next = pmm_allocator.freelist;
  pmm_allocator.freelist = block;
}

void
pmm_test(void)
{
  void *p = (void *)pmm_alloc();
  printf("p  = %p\n", p);
  // pmm_free(p);

  void *p2 = (void *)pmm_alloc();
  printf("p2 = %p\n", p2);
  pmm_free((phys_addr_t)p2);

  void *p3 = (void *)pmm_alloc();
  printf("p3 = %p\n", p3);
}
