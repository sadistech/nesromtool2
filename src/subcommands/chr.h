#ifndef _NRT_SUBCOMMANDS_CHR_H_
#define _NRT_SUBCOMMANDS_CHR_H_

#include <getopt.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void subcommand_chr(int, char**);
void subcommand_chr_extract(int, char**);
void subcommand_chr_replace(int, char**);

#endif
