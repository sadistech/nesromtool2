#ifndef _HELP_H_
#define _HELP_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ANSI_COLOR_BOLD    "\x1b[1m"
#define ANSI_COLOR_UNBOLD  "\x1b[22m"

#define ANSI_COLOR_RED     "\x1b[0;31m"
#define ANSI_COLOR_GREEN   "\x1b[0;32m"
#define ANSI_COLOR_YELLOW  "\x1b[0;33m"
#define ANSI_COLOR_BLUE    "\x1b[0;34m"
#define ANSI_COLOR_MAGENTA "\x1b[0;35m"
#define ANSI_COLOR_CYAN    "\x1b[0;36m"

#define ANSI_COLOR_BRED     "\x1b[1;31m"
#define ANSI_COLOR_BGREEN   "\x1b[1;32m"
#define ANSI_COLOR_BYELLOW  "\x1b[1;33m"
#define ANSI_COLOR_BBLUE    "\x1b[1;34m"
#define ANSI_COLOR_BMAGENTA "\x1b[1;35m"
#define ANSI_COLOR_BCYAN    "\x1b[1;36m"

#define ANSI_COLOR_RESET   "\x1b[0m"

#define HELP_HEADER_COLOR ANSI_COLOR_BCYAN

#define VERSION "2.0.0-alpha"
#define APPNAME "nrt"

#define HELP_ALLOC_OPTS (help_opts*)calloc(sizeof(help_opts), 1)
#define NULL_COL { NULL, NULL }

typedef struct help_opts {
  unsigned int indent;
} help_opts;

typedef struct help_cols {
  char *usage;
  char *desc;
} help_cols;

void p_header(char *);
void p_newline(void);
void p_indent(int level, char *str);
void p_section(char *header, char *usage, struct help_cols *fields, struct help_cols *out_opts);
void p_cols(help_opts *opts, struct help_cols *cols);

int widest_cols_left(struct help_cols *cols);

void print_version();

void print_usage();
void print_usage_info();

void print_usage_prg();
void print_usage_prg_extract();
void print_usage_prg_replace();

void print_usage_chr();
void print_usage_chr_extract();
void print_usage_chr_replace();

void print_usage_title();

#endif
