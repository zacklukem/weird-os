#ifndef INCLUDES_ASSERT
#define INCLUDES_ASSERT

// Assert macro and definition
#define assert(expr)                                                           \
  if (!(expr))                                                                 \
  __assert(#expr, __FILE__, __LINE__)

void __assert(const char *msg, const char *file, int line);

#endif // INCLUDES_ASSERT
