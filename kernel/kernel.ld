/* 指定指令集架构 */
OUTPUT_ARCH(riscv)

/* 指定入口点为 entry.S 中的 _entry */
ENTRY(_entry)

/* 内核被加载到内存中的起始地址 */
BASE_ADDRESS_ = 0x80200000;

/* 内核镜像 ELF 文件中各段的相关配置 */
SECTIONS
{
  /* 指定起始位置为 BASE_ADDRESS_ */
  . = BASE_ADDRESS_;

  /* 代码段按 4K 对齐 */
  .text : {
    *(.text.entry)
    *(.text .text.*)
    . = ALIGN(0x1000);
  }

  /* 只读数据段 */
  .rodata : {
    . = ALIGN(16);
    *(.srodata .srodata.*)
    . = ALIGN(16);
    *(.rodata .rodata.*)
  }

  /* 数据段 */
  .data : {
    . = ALIGN(16);
    *(.sdata .sdata.*)
    . = ALIGN(16);
    *(.data .data.*)
  }

  /* BSS段 */
  .bss : {
    . = ALIGN(16);
    *(.bss.stack)
    . = ALIGN(16);
    *(.sbss .sbss.*)
    . = ALIGN(16);
    *(.bss .bss.*)
  }

  /* 去掉一些不需要的段 */
  /DISCARD/ : {
    *(.eh_frame)
  }
}
