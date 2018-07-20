#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TEMP_DIR_ENV      "NRT_TEMP"
#define FIXTURE_DIR_ENV   "NRT_FIXTURE_DIR"
#define TEST_DIR_ENV      "NRT_TEST_DIR"

char *temp_dir;
char *fixture_dir;
char *test_dir;

void test_init() {
  temp_dir = getenv(TEMP_DIR_ENV);
  fixture_dir = getenv(FIXTURE_DIR_ENV);
  test_dir = getenv(TEST_DIR_ENV);
}

// join 2 paths, but return a newly allocated string
// so the user has to be sure to free it.
char *path_join(char *basepath, char *extra_path) {
  char *newpath = (char*)calloc(1, strlen(basepath) + strlen(extra_path) + 1);

  strcpy(newpath, basepath);

  if (newpath[strlen(basepath) - 1] != '/') {
    newpath[strlen(basepath)] = '/';
  }

  strcat(newpath, extra_path);

  return newpath;
}
