#ifndef _SUBCOMMANDS_TITLE_H_
#define _SUBCOMMANDS_TITLE_H_

#include <getopt.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "../util.h"
#include "../nrt.h"
#include "../subcommands.h"

typedef struct title_opts {
  char romfile_path[PATH_LENGTH];

  char title[NRT_TITLE_MAX_LENGTH];

  char outfile_path[PATH_LENGTH]; // rom file that we're gonna write to.
} title_opts;

title_opts* subcommand_title_set_parse(int, char**);
title_opts* subcommand_title_get_parse(int, char**);
title_opts* subcommand_title_remove_parse(int, char**);

void subcommand_title(int, char**);

void subcommand_title_set(title_opts *opts);
void subcommand_title_get(title_opts *opts);
void subcommand_title_remove(title_opts *opts);

#endif
