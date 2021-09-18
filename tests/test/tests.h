#ifndef TESTS_TESTS_H
#define TESTS_TESTS_H
#include <stdio.h>

struct __test_result__ {
  int fail;
  int line;
  const char *file;
  const char *cond;
  const char *message;
};

#define TEST_CASE(test_name)                                                   \
  extern "C" void test_name(struct __test_result__ *__result__)

static inline int __test_assert__(struct __test_result__ *result, int cond,
                                  int line, const char *file,
                                  const char *cond_s, const char *message) {
  if (!cond) {
    result->fail = 1;
    result->line = line;
    result->file = file;
    result->cond = cond_s;
    result->message = message;
    return 1;
  }
  return 0;
}

#define ASSERT(cond)                                                           \
  {                                                                            \
    if (__test_assert__(__result__, (cond), __LINE__, __FILE__, #cond, ""))    \
      return;                                                                  \
  }

#define ASSERT_EQ(a, b)                                                        \
  {                                                                            \
    int a_val = (a);                                                           \
    int b_val = (b);                                                           \
    bool cond = a_val == b_val;                                                \
    char buf[256];                                                             \
    buf[sprintf(buf, "%d != %d", a_val, b_val)] = 0;                           \
    if (!cond) {                                                               \
      __test_assert__(__result__, (cond), __LINE__, __FILE__, #a " == " #b,    \
                      buf);                                                    \
      return;                                                                  \
    }                                                                          \
  }

#define DEFINE_TEST_CASE(test_name)                                            \
  extern "C" void test_name(struct __test_result__ *__result__);

#define RUN_TEST_CASE(test_name) __run_test_case__(test_name, #test_name);

#endif // TESTS_TESTS_H
