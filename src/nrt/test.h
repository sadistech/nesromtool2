#ifndef _NRT_TEST_H_
#define _NRT_TEST_H_

#define test(name) bool test_ ## name(void)
#define it(msg, func) run_it(msg, &test_ ## func)

char *context;
int counter = 0;
int fail_count = 0;

void describe(char *thing) {
  context = thing;
  printf("%s:\n", context);
}

void run_it(char *desc, bool (*func)(void)) {
  char *result_text;
  bool result = func();

  counter++;

  if (result) {
    result_text = "PASS";
  } else {
    result_text = "FAIL";
    fail_count++;
  }

  printf("  [%s] %s\n", result_text, desc);
}

void print_summary() {
  printf("\n");
  printf("Tests completed.\n");
  printf("  %d tests run.\n", counter);
  printf("  %d failures\n", fail_count);

  if (fail_count > 0) {
    exit(EXIT_FAILURE);
  }
}

#endif
