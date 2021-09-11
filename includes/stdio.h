#ifndef INCLUDES_STDIO
#define INCLUDES_STDIO

#include <stdarg.h>

struct struct_FILE {

  char *_IO_read_ptr;   // Current read pointer
  char *_IO_read_end;   // End of get area.
  char *_IO_read_base;  // Start of putback+get area.
  char *_IO_write_base; // Start of put area.
  char *_IO_write_ptr;  // Current put pointer.
  char *_IO_write_end;  // End of put area.
  char *_IO_buf_base;   // Start of reserve area.
  char *_IO_buf_end;    // End of reserve area.

  int (*_flush_func)();
};

typedef struct struct_FILE FILE;

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

// void clearerr(FILE *);
// char *ctermid(char *);
// char *cuserid(char *);
// int fclose(FILE *);
// FILE *fdopen(int, const char *);
// int feof(FILE *);
// int ferror(FILE *);
int fflush(FILE *);
// int fgetc(FILE *);
// int fgetpos(FILE *, fpos_t *);
// char *fgets(char *, int, FILE *);
// int fileno(FILE *);
// void flockfile(FILE *);
// FILE *fopen(const char *, const char *);
int fprintf(FILE *, const char *, ...);
int fputc(int, FILE *);
int fputs(const char *, FILE *);
// size_t fread(void *, size_t, size_t, FILE *);
// FILE *freopen(const char *, const char *, FILE *);
// int fscanf(FILE *, const char *, ...);
// int fseek(FILE *, long int, int);
// int fseeko(FILE *, off_t, int);
// int fsetpos(FILE *, const fpos_t *);
// long int ftell(FILE *);
// off_t ftello(FILE *);
// int ftrylockfile(FILE *);
// void funlockfile(FILE *);
// size_t fwrite(const void *, size_t, size_t, FILE *);
int getc(FILE *);
int getchar(void);
// int getc_unlocked(FILE *);
// int getchar_unlocked(void);
// int getopt(int, char *const[], const char);
// (LEGACY) char *gets(char *);
// int getw(FILE *);
// int pclose(FILE *);
// void perror(const char *);
// FILE *popen(const char *, const char *);
int printf(const char *, ...);
int putc(int, FILE *);
int putchar(int);
// int putc_unlocked(int, FILE *);
// int putchar_unlocked(int);
int puts(const char *);
// int putw(int, FILE *);
// int remove(const char *);
// int rename(const char *, const char *);
// void rewind(FILE *);
// int scanf(const char *, ...);
// void setbuf(FILE *, char *);
// int setvbuf(FILE *, char *, int, size_t);
// int snprintf(char *, size_t, const char *, ...);
// int sprintf(char *, const char *, ...);
// int sscanf(const char *, const char *, int...);
// char *tempnam(const char *, const char *);
// FILE *tmpfile(void);
// char *tmpnam(char *);
// int ungetc(int, FILE *);
int vfprintf(FILE *, const char *, va_list);
int vprintf(const char *, va_list);
// int vsnprintf(char *, size_t, const char *, va_list);
// int vsprintf(char *, const char *, va_list);

#endif // INCLUDES_STDIO
