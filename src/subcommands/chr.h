#ifndef _NRT_SUBCOMMANDS_CHR_H_
#define _NRT_SUBCOMMANDS_CHR_H_

#include <getopt.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "../util.h"
#include "../nrt.h"

#define PATH_LENGTH 1024

typedef enum output_format {
  null_format,
  raw_format,
  png_format
} output_format;

typedef struct chr_opts {
  output_format format;

  int width; // width in tiles
  int bank_index; // index of target bank

  char outfile_path[PATH_LENGTH]; // path to the file we're writing to
  char romfile_path[PATH_LENGTH]; // path to rom that we're reading from
} chr_opts;

output_format parse_output_format(char *format);
output_format filename_to_output_format(char *filename);

chr_opts* subcommand_chr_extract_parse(int, char**);
chr_opts* subcommand_chr_replace_parse(int, char**);

void subcommand_chr(int, char**);

void subcommand_chr_extract(chr_opts *opts);
void subcommand_chr_replace(int, char**);


#endif
