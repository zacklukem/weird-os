#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

FILE *stdin;
FILE *stdout;
FILE *stderr;

int fflush(FILE *file) { return file->_flush_func(); }

int getc(FILE *f) {
  while (!*f->_IO_read_ptr)
    __asm__("hlt");
  char tmp = *f->_IO_read_ptr;
  *f->_IO_read_ptr = 0;
  f->_IO_read_ptr--;
  return tmp;
}

int getchar() { return getc(stdin); }

int fputc(int c, FILE *file) {
  *file->_IO_write_ptr++ = c;
  fflush(file);
  return c;
}

int fputs(const char *str, FILE *f) {
  for (int i = 0; str[i] != 0; ++i) {
    fputc(str[i], f);
  }
  return 0;
}

int putc(int i, FILE *f) {
  fputc(i, f);
  return i;
}

int putchar(int c) { return putc(c, stdout); }

int puts(const char *str) { return fputs(str, stdout); }

int vfprintf(FILE *f, const char *format, va_list args) {

  for (int i = 0; format[i] != 0; ++i) {
    if (format[i] == '%') {
      char next = format[++i];
      assert(next != 0 && "Invalid printf argument");

      switch (next) {
      case '%':
        fputc('%', f);
        break;
      case 'd': {
        int value = va_arg(args, int);
        char buf[16];
        itoa(value, buf, 10);
        fputs(buf, f);
      } break;
      case 'x': {
        uint32_t value = va_arg(args, uint32_t);
        uint32_t tmp_value = value;
        char buf[16];

        for (int i = 7; i >= 0; --i) {
          char v = tmp_value & 0xf;
          buf[i] = v + (v < 10 ? '0' : ('a' - 10));
          tmp_value >>= 4;
        }
        buf[8] = 0;
        fputs(buf, f);
      } break;
      case 's': {
        char *value = va_arg(args, char *);
        fputs(value, f);
      } break;
      }
    } else {
      fputc(format[i], f);
    }
  }

  return 0;
}

int vprintf(const char *format, va_list args) {
  return vfprintf(stdout, format, args);
}

int fprintf(FILE *f, const char *format, ...) {
  va_list args;
  va_start(args, format);
  return vfprintf(f, format, args);
}

int printf(const char *format, ...) {
  va_list args;
  va_start(args, format);
  return vfprintf(stdout, format, args);
}