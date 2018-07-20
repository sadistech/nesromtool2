#include "subcommands.h"

bool is_subcommand(char *subcommand, ...) {
  va_list aliases;

  va_start(aliases, subcommand);

  char *alias = va_arg(aliases, char*);

  while(alias) {
    if (strcmp(alias, subcommand) == 0) {
      va_end(aliases);
      return true;
    }

    alias = va_arg(aliases, char*);
  }

  va_end(aliases);

  return false;
}
