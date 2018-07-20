#include "subcommands.h"

bool is_subcommand(char *subcommand, char **subcommand_list) {
  char *c = subcommand_list[0];

  while(c) {
    if (strcmp(subcommand, c) == 0) {
      return true;
    }

    c = subcommand++;
  }

  return false;
}
