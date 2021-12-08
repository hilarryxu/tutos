#ifndef TUTOS_MACROS_H_
#define TUTOS_MACROS_H_

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

#endif  // TUTOS_MACROS_H_
