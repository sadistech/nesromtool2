#include "help.h"

void p_header(char *header) {
  printf(HELP_HEADER_COLOR "%s:" ANSI_COLOR_RESET "\n", header);
}

void p_newline(void) {
  printf("\n");
}

void print_cols(help_opts *opts, struct help_cols *cols) {
  struct help_cols *t = cols;
  int usage_width = widest_cols_left(cols);
  char indent_string[256] = "";
  bzero(indent_string, 256);
  int i = 0;

  for (i = 0; i < opts->indent; i++) {
    // 2-space indent
    indent_string[i] = ' ';
    indent_string[i * 2 + 1] = ' ';
  }

  char fmt_string[256] = "";

  sprintf(fmt_string, "%s%%-%ds %%s\n", indent_string, usage_width + 2);

  while(t->usage) {
    printf(fmt_string, t->usage, t->desc);
    t = ++cols;
  }
}

int widest_cols_left(struct help_cols *cols) {
  struct help_cols *t = cols;
  int longest = 0;
  int current = 0;

  while(t->usage) {
    current = strlen(t->usage);
    if (current > longest) {
      longest = current;
    }

    t = ++cols;
  }

  return longest;
}

void print_indent(int level, char *str) {
  char *token, *tofree;
  char indent_string[256] = "";
  bzero(indent_string, 256);

  int i = 0;

  for (i = 0; i < level; i++) {
    indent_string[i] = ' ';
    indent_string[i * 2 + 1] = ' ';
  }

  tofree = strdup(str);

  while((token = strsep(&str, "\n")) != NULL) {
    printf("%s%s\n", indent_string, token);
  }

  free(tofree);
}

void print_help_header(char *appname, char *description) {
  p_header(appname);
  print_indent(1, description);
  p_newline();
}

void print_help_usage(char *appname, char *usage) {
  p_header("Usage");
  print_indent(1, usage);
  p_newline();
}

void print_section(char *header, char *usage, struct help_cols *fields, struct help_cols *out_opts) {
  help_opts *opts = HELP_ALLOC_OPTS;
  opts->indent = 1;

  p_header(header);

  print_indent(1, usage);

  if (fields) {
    p_header("Fields");
    print_cols(opts, fields);
  }

  if (out_opts) {
    p_header("Options");
    print_cols(opts, out_opts);
  }
}

void print_version() {
  printf("%s\n", VERSION);
}

void print_usage() {
  help_opts *opts = HELP_ALLOC_OPTS;
  opts->indent = 1;

  print_help_header(APPNAME, "A tool for working with NES ROM files in the iNES v1 format.");
  print_help_usage(APPNAME, "<subcommand> [ <subcommand-args> ... ]");


  p_header("Subcommands");

  static struct help_cols subcommand_table[] = {
    { "help", "Print the usage for the given subcommand" },
    { "info", "Print an information dump about the given rom" },
    { "prg", "Perform PRG bank actions such as extraction and replacement" },
    { "chr", "Perform CHR bank actions such as extraction and replacement" },
    { "title", "Perform Title metadata actions such as reading and setting" },
    NULL_COL
  };

  print_cols(opts, subcommand_table);

  p_newline();

  printf(
    "For usage information on the above subcommands run `%s help <subcommand>'\n",
    APPNAME
  );

  p_newline();
}

void print_usage_info() {
  help_opts *opts = HELP_ALLOC_OPTS;
  opts->indent = 1;

  printf(
    "\n"
    "%s info\n"
    "  Dump all information about the given ROM. This includes a validation\n"
    "  step, mapper information, number of PRG and CHR banks and title (if it\n"
    "  has one)\n"
    "\n"
    "Usage:"
    "  %s info <rom>\n"
    "\n",
    APPNAME, APPNAME
  );
}

void print_usage_prg() {
  help_opts *opts = HELP_ALLOC_OPTS;
  opts->indent = 1;

  printf(
    "\n"
    "%s prg\n"
    "\n"
    "\n",
    APPNAME
  );

  static struct help_cols extraction_fields[] = {
    { "<rom>", "Path to the .nes target ROM file" },
    { "<bank-index>", "Zero-based index of target PRG bank to extract" },
    { "<outfile>", "Path to the file to write the extracted PRG bank" },
    NULL_COL
  };

  static struct help_cols extraction_options[] = {
    { "-f | --format <format>", "The format of the output file. Currently can only be 'raw'" },
    NULL_COL
  };

  print_section(
    "Extraction",
    "nrt prg extract <rom> [ <options> ... ] <bank-index> <outfile>",
    extraction_fields,
    extraction_options
  );
}

void print_usage_chr() {
  help_opts *opts = HELP_ALLOC_OPTS;
  opts->indent = 1;

  printf(
    "\n"
    "%s info\n"
    "  Dump all information about the given ROM. This includes a validation\n"
    "  step, mapper information, number of PRG and CHR banks and title (if it\n"
    "  has one)\n"
    "\n"
    "Usage:"
    "  %s info <rom>\n"
    "\n",
    APPNAME, APPNAME
  );

  printf("Actions:\n");

  static struct help_cols subcommand_table[] = {
    { NULL, NULL }
  };

  print_cols(opts, subcommand_table);
}

void print_usage_title() {
  help_opts *opts = HELP_ALLOC_OPTS;
  opts->indent = 1;

  printf(
    "\n"
    "%s info\n"
    "  Dump all information about the given ROM. This includes a validation\n"
    "  step, mapper information, number of PRG and CHR banks and title (if it\n"
    "  has one)\n"
    "\n"
    "Usage:"
    "  %s info <rom>\n"
    "\n",
    APPNAME, APPNAME
  );

  printf("Actions:\n");

  static struct help_cols subcommand_table[] = {
    { NULL, NULL }
  };

  print_cols(opts, subcommand_table);
}

