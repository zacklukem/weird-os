#include <stdint.h>
#include <unistd.h>

#define PP_NARG(...) PP_NARG_(__VA_ARGS__, PP_RSEQ_N())
#define PP_NARG_(...) PP_ARG_N(__VA_ARGS__)
#define PP_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14,  \
                 _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26,   \
                 _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38,   \
                 _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50,   \
                 _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62,   \
                 _63, N, ...)                                                  \
  N
#define PP_RSEQ_N()                                                            \
  63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45,  \
      44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27,  \
      26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9,   \
      8, 7, 6, 5, 4, 3, 2, 1, 0

static_assert(PP_NARG(1) == 1);
static_assert(PP_NARG(1, 2) == 2);
static_assert(PP_NARG(1, 2, 3) == 3);
static_assert(PP_NARG(1, 2, 3, 4) == 4);
static_assert(PP_NARG(1, 2, 3, 4, 5) == 5);
static_assert(PP_NARG(1, 2, 3, 4, 5, 6) == 6);
static_assert(PP_NARG(1, 2, 3, 4, 5, 6, 7) == 7);

// %eax for syscall_number. %ebx, %ecx, %edx, %esi, %edi, %ebp

static inline int syscall(uint32_t num) {
  uint32_t eax;
  asm volatile("int $0x80" : "=a"(eax) : "a"(num));
  return eax;
}

template <class T> static inline int syscall(uint32_t num, T _0) {
  uint32_t eax;
  asm volatile("int $0x80" : "=a"(eax) : "a"(num), "b"((uint32_t)_0));
  return eax;
}

template <class T, class U>
static inline int syscall(uint32_t num, T _0, U _1) {
  uint32_t eax;
  asm volatile("int $0x80"
               : "=a"(eax)
               : "a"(num), "b"((uint32_t)_0), "c"((uint32_t)_1));
  return eax;
}

template <class T, class U, class V>
static inline int syscall(uint32_t num, T _0, U _1, V _2) {
  uint32_t eax;
  asm volatile("int $0x80"
               : "=a"(eax)
               : "a"(num), "b"((uint32_t)_0), "c"((uint32_t)_1),
                 "d"((uint32_t)_2));
  return eax;
}

template <class T, class U, class V, class W>
static inline int syscall(uint32_t num, T _0, U _1, V _2, W _3) {
  uint32_t eax;
  asm volatile("int $0x80"
               : "=a"(eax)
               : "a"(num), "b"((uint32_t)_0), "c"((uint32_t)_1),
                 "d"((uint32_t)_2), "S"((uint32_t)_3));
  return eax;
}

template <class T, class U, class V, class W, class X>
static inline int syscall(uint32_t num, T _0, U _1, V _2, W _3, X _4) {
  uint32_t eax;
  asm volatile("int $0x80"
               : "=a"(eax)
               : "a"(num), "b"((uint32_t)_0), "c"((uint32_t)_1),
                 "d"((uint32_t)_2), "S"((uint32_t)_3), "D"((uint32_t)_4));
  return eax;
}

ssize_t write(int fildes, const void *buf, size_t size) {
  return syscall(3, fildes, buf, size);
}

ssize_t read(int fildes, void *buf, size_t size) {
  return syscall(2, fildes, buf, size);
}
