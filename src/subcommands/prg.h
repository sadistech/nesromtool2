#ifndef _SUBCOMMANDS_PRG_H_
#define _SUBCOMMANDS_PRG_H_

#include <getopt.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "../util.h"
#include "../nrt.h"
#include "../subcommands.h"

typedef enum prg_output_format {
  prg_null_format,
  prg_raw_format
} prg_output_format;

typedef struct prg_opts {
  char romfile_path[PATH_LENGTH]; // path to rom that we're reading from

  prg_output_format format;

  int bank_index; // index of target bank

  char sourcefile_path[PATH_LENGTH]; // path to the target data file
  char outfile_path[PATH_LENGTH]; // path to the file we're writing to
} prg_opts;

prg_output_format parse_prg_output_format(char *format);
prg_output_format filename_to_prg_output_format(char *filename);

prg_opts* subcommand_prg_extract_parse(int, char**);
prg_opts* subcommand_prg_replace_parse(int, char**);

void subcommand_prg(int, char**);

void subcommand_prg_extract(prg_opts *opts);
void subcommand_prg_replace(prg_opts *opts);

#endif

