#ifndef _SUBCOMMANDS_INFO_H_
#define _SUBCOMMANDS_INFO_H_

#include <getopt.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "../util.h"
#include "../nrt.h"
#include "../subcommands.h"

typedef struct info_opts {
  char romfile_path[PATH_LENGTH]; // path to rom that we're reading from
} info_opts;

info_opts* subcommand_info_parse(int, char**);

void subcommand_info(int, char**);

#endif
