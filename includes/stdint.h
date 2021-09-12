#ifndef INCLUDES_STDINT
#define INCLUDES_STDINT

typedef unsigned char int8_t;
_Static_assert(sizeof(int8_t) == 1, "int8_t is not 1 byte");

typedef unsigned short int16_t;
_Static_assert(sizeof(int16_t) == 2, "int16_t is not 2 bytes");

typedef unsigned int int32_t;
_Static_assert(sizeof(int32_t) == 4, "int32_t is not 4 bytes");

typedef unsigned char uint8_t;
_Static_assert(sizeof(uint8_t) == 1, "uint8_t is not 1 byte");

typedef unsigned short uint16_t;
_Static_assert(sizeof(uint16_t) == 2, "uint16_t is not 2 bytes");

typedef unsigned int uint32_t;
_Static_assert(sizeof(uint32_t) == 4, "uint32_t is not 4 bytes");

typedef unsigned int size_t;
_Static_assert(sizeof(size_t) == sizeof(void *),
               "size_t is not equal to pointer size");

typedef int ssize_t;
_Static_assert(sizeof(ssize_t) == sizeof(void *),
               "ssize_t is not equal to pointer size");

#endif // INCLUDES_STDINT
