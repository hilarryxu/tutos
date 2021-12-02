#ifndef TUTOS_SBI_H_
#define TUTOS_SBI_H_

#include "defs.h"

#define SBI_SET_TIMER 0
#define SBI_CONSOLE_PUTCHAR 1
#define SBI_CONSOLE_GETCHAR 2
#define SBI_CLEAR_IPI 3
#define SBI_SEND_IPI 4
#define SBI_REMOTE_FENCE_I 5
#define SBI_REMOTE_SFENCE_VMA 6
#define SBI_REMOTE_SFENCE_VMA_ASID 7
#define SBI_SHUTDOWN 8

#define SBI_CALL(which, arg0, arg1, arg2, arg3)           \
  ({                                                      \
    register uintptr_t a0 asm("a0") = (uintptr_t)(arg0);  \
    register uintptr_t a1 asm("a1") = (uintptr_t)(arg1);  \
    register uintptr_t a2 asm("a2") = (uintptr_t)(arg2);  \
    register uintptr_t a3 asm("a3") = (uintptr_t)(arg3);  \
    register uintptr_t a7 asm("a7") = (uintptr_t)(which); \
    asm volatile("ecall"                                  \
                 : "+r"(a0)                               \
                 : "r"(a1), "r"(a2), "r"(a3), "r"(a7)     \
                 : "memory");                             \
    a0;                                                   \
  })

#define SBI_CALL_0(which) SBI_CALL(which, 0, 0, 0, 0)
#define SBI_CALL_1(which, arg0) SBI_CALL(which, arg0, 0, 0, 0)
#define SBI_CALL_2(which, arg0, arg1) SBI_CALL(which, arg0, arg1, 0, 0)
#define SBI_CALL_3(which, arg0, arg1, arg2) SBI_CALL(which, arg0, arg1, arg2, 0)
#define SBI_CALL_4(which, arg0, arg1, arg2, arg3) \
  SBI_CALL(which, arg0, arg1, arg2, arg3)

static inline void
sbi_set_timer(uint64_t cycle)
{
  SBI_CALL_1(SBI_SET_TIMER, cycle);
}

static inline void
sbi_console_putchar(int ch)
{
  SBI_CALL_1(SBI_CONSOLE_PUTCHAR, ch);
}

static inline int
sbi_console_getchar(void)
{
  return SBI_CALL_0(SBI_CONSOLE_GETCHAR);
}

static inline void
sbi_shutdown(void)
{
  SBI_CALL_0(SBI_SHUTDOWN);
  panic("shutdown");
}

#endif  // TUTOS_SBI_H_
