#ifndef _SUBCOMMANDS_CHR_H_
#define _SUBCOMMANDS_CHR_H_

#include <getopt.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "../help.h"
#include "../util.h"
#include "../nrt.h"
#include "../subcommands.h"

typedef enum chr_output_format {
  null_format,
  raw_format,
  png_format
} chr_output_format;

typedef struct chr_opts {
  char romfile_path[PATH_LENGTH]; // path to rom that we're reading from

  chr_output_format format;

  int width; // width in tiles
  int bank_index; // index of target bank

  char sourcefile_path[PATH_LENGTH]; // path to the target data file
  char outfile_path[PATH_LENGTH]; // path to the file we're writing to

} chr_opts;

chr_output_format parse_chr_output_format(char *format);
chr_output_format filename_to_chr_output_format(char *filename);

chr_opts* subcommand_chr_extract_parse(int, char**);
chr_opts* subcommand_chr_replace_parse(int, char**);

void subcommand_chr(int, char**);

void subcommand_chr_extract(chr_opts *opts);
void subcommand_chr_replace(chr_opts *opts);


#endif
