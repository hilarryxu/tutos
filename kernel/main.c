#include "defs.h"

extern char _bss_partial_start[];
extern char _bss_end[];

// 清零内核的 .bss 段，保证访问该区域的变量符合预期的空值
// NOTE: 这里我们只处理 [bss_partial_start, bss_end) 区域
//   .bss.stack 是栈空间千万不能也给清零了
static void
clear_bss(void)
{
  bzero(_bss_partial_start, _bss_end - _bss_partial_start);
}

//---------------------------------------------------------------------
// 内核入口
//---------------------------------------------------------------------
void
kmain(void)
{
  // 清零 BSS 段
  clear_bss();

  printf("Hello, TUTOS!\n");
}
