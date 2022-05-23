# 启动流程:
#   机器加电自检，执行 ROM 中一段程序后跳转至
#     -> 0x8000_0000  QEMU 自带的 OpenSBI（-bios default）
#         -> 0x8020_0000  我们的内核镜像 kernel.img (-kernel build/kernel.img)
#
# NOTE: 我们借助 QEMU -kernel 选项让模拟器将我们的 ELF 格式的内核镜像
#   加载到指定位置，真机上就不行了，需要在 OpenSBI 和内核这个流程中再
#   插入一个 bootloader 去识别并加载 ELF 格式的内核镜像。

# 内核的代码段要放到 0x8020_0000 处
.section .text.entry
.global _entry
_entry:
  # 设置 sp 栈指针寄存器为内核栈顶位置
  la sp, boot_stack_top
  # 准备调用内核主函数 kmain
  call kmain

  # 内核返回后 CPU 在这里 wfi 低功耗等待中断
1:
  # 用 wfi 指令避免空转消耗 CPU（低功耗运转并等待中断）
  wfi
  j 1b

# FIXME(xcc): 需要 4K 页对齐不？
.section .bss.stack
.global boot_stack
boot_stack:
  .space 4096  # 4KB 大小的内核栈
.global boot_stack_top
boot_stack_top:
