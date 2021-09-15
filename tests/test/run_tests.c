#include "tests.h"
#include <kernel/printk.h>
#include <stdio.h>

static int num_failed = 0;
static int num_total = 0;
static int num_pass = 0;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
static void __run_test_case__(void (*test)(struct __test_result__ *),
                              const char *test_name) {
  num_total++;
  struct __test_result__ result;
  printf("Testing %s... ", test_name);
  result.fail = 0;
  test(&result);
  if (result.fail) {
    num_failed++;
    printf("\nFAILED AT: %s:%d:\n    %s\n", result.file, result.line,
           result.message);
  } else {
    num_pass++;
    printf("OK\n");
  }
}
#pragma GCC diagnostic pop

void run_kernel_tests() {
  cleark();
#include "list_gen.h"
  if (num_failed) {
    printf("%d tests FAILED of %d\n", num_failed, num_total);
  } else {
    printf("%d tests passed of %d\n", num_pass, num_total);
  }
}