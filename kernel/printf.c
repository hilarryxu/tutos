#include "defs.h"
#include "sbi.h"

#include <stdarg.h>

static void
consputc(int c)
{
  sbi_console_putchar(c);
}

static char digits[] = "0123456789abcdef";

static void
printint(int xx, int base, int sign)
{
  char buf[16];
  int i;
  unsigned int x;

  if (sign && (sign = xx < 0))
    x = -xx;
  else
    x = xx;

  i = 0;
  do {
    buf[i++] = digits[x % base];
  } while ((x /= base) != 0);

  if (sign)
    buf[i++] = '-';

  while (--i >= 0)
    consputc(buf[i]);
}

static void
printptr(u64 x)
{
  int i;
  consputc('0');
  consputc('x');
  for (i = 0; i < (sizeof(u64) * 2); i++, x <<= 4)
    consputc(digits[x >> (sizeof(u64) * 8 - 4)]);
}

// Print to the console. only understands %d, %x, %p, %s.
void
printf(char *fmt, ...)
{
  va_list ap;
  int i, c;
  char *s;

  if (fmt == 0)
    panic("null fmt");

  va_start(ap, fmt);
  for (i = 0; (c = fmt[i] & 0xff) != 0; i++) {
    if (c != '%') {
      consputc(c);
      continue;
    }
    c = fmt[++i] & 0xff;
    if (c == 0)
      break;
    switch (c) {
    case 'c':
      consputc(va_arg(ap, int));
      break;
    case 'd':
      printint(va_arg(ap, int), 10, 1);
      break;
    case 'u':
      printint(va_arg(ap, int), 10, 0);
      break;
    case 'x':
      printint(va_arg(ap, int), 16, 1);
      break;
    case 'p':
      printptr(va_arg(ap, u64));
      break;
    case 's':
      if ((s = va_arg(ap, char *)) == 0)
        s = "(null)";
      for (; *s; s++)
        consputc(*s);
      break;
    case '%':
      consputc('%');
      break;
    default:
      // Print unknown % sequence to draw attention.
      consputc('%');
      consputc(c);
      break;
    }
  }
  va_end(ap);
}

void
panic(char *s)
{
  printf("PANIC: ");
  printf(s);
  printf("\n");

  for (;;)
    ;
}
