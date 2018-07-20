#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

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

// copy a file
// returns 0 on success; -1 on failure.
// code from:
// https://stackoverflow.com/questions/2180079/how-can-i-copy-a-file-on-unix-using-c/2180788#2180788
int copy_file(char *to, char *from) {
  int fd_to, fd_from;
  char buf[4096];
  ssize_t nread;
  int saved_errno;

  fd_from = open(from, O_RDONLY);
  if (fd_from < 0)
    return -1;

  fd_to = open(to, O_WRONLY | O_CREAT | O_EXCL, 0666);
  if (fd_to < 0)
    goto out_error;

  while (nread = read(fd_from, buf, sizeof buf), nread > 0)
  {
    char *out_ptr = buf;
    ssize_t nwritten;

    do {
      nwritten = write(fd_to, out_ptr, nread);

      if (nwritten >= 0)
      {
        nread -= nwritten;
        out_ptr += nwritten;
      }
      else if (errno != EINTR)
      {
        goto out_error;
      }
    } while (nread > 0);
  }

  if (nread == 0)
  {
    if (close(fd_to) < 0)
    {
      fd_to = -1;
      goto out_error;
    }
    close(fd_from);

    /* Success! */
    return 0;
  }

  out_error:
  saved_errno = errno;

  close(fd_from);
  if (fd_to >= 0)
    close(fd_to);

  errno = saved_errno;
  return -1;
}
