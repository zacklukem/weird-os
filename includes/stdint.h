#ifndef INCLUDES_STDINT
#define INCLUDES_STDINT

typedef unsigned char int8_t;
static_assert(sizeof(int8_t) == 1, "int8_t is not 1 byte");

typedef unsigned short int16_t;
static_assert(sizeof(int16_t) == 2, "int16_t is not 2 bytes");

typedef unsigned int int32_t;
static_assert(sizeof(int32_t) == 4, "int32_t is not 4 bytes");

typedef unsigned char uint8_t;
static_assert(sizeof(uint8_t) == 1, "uint8_t is not 1 byte");

typedef unsigned short uint16_t;
static_assert(sizeof(uint16_t) == 2, "uint16_t is not 2 bytes");

typedef unsigned long uint32_t;
static_assert(sizeof(uint32_t) == 4, "uint32_t is not 4 bytes");

typedef unsigned long size_t;
static_assert(sizeof(size_t) == sizeof(void *),
              "size_t is not equal to pointer size");

typedef int ssize_t;
static_assert(sizeof(ssize_t) == sizeof(void *),
              "ssize_t is not equal to pointer size");

typedef uint32_t uid_t;
typedef uint32_t ino_t;
typedef uint32_t gid_t;
typedef uint32_t off_t;
typedef uint32_t dev_t;
typedef uint32_t mode_t;
typedef uint32_t nlink_t;
typedef uint32_t time_t;
typedef uint32_t blksize_t;
typedef uint32_t blkcnt_t;

#endif // INCLUDES_STDINT
