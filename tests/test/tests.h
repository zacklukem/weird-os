#ifndef TESTS_TESTS_H
#define TESTS_TESTS_H

struct __test_result__ {
  int fail;
  int line;
  const char *file;
  const char *message;
};

#define TEST_CASE(test_name) void test_name(struct __test_result__ *__result__)

static inline int __test_assert__(struct __test_result__ *result, int cond,
                                  int line, const char *file,
                                  const char *message) {
  if (!cond) {
    result->fail = 1;
    result->line = line;
    result->file = file;
    result->message = message;
    return 1;
  }
  return 0;
}

#define ASSERT(cond)                                                           \
  {                                                                            \
    if (__test_assert__(__result__, (cond), __LINE__, __FILE__, #cond))        \
      return;                                                                  \
  }

#define RUN_TEST_CASE(test_name)                                               \
  extern void test_name(struct __test_result__ *__result__);                   \
  __run_test_case__(test_name, #test_name);

#endif // TESTS_TESTS_H
