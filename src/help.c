#include "help.h"

void p_header(char *header) {
  printf(HELP_HEADER_COLOR "%s:" ANSI_COLOR_RESET "\n", header);
}

void p_newline(void) {
  printf("\n");
}

void p_indent(int level, char *str) {
  char *token, *newstr, *tofree;
  char indent_string[256] = "";
  bzero(indent_string, 256);

  tofree = newstr = strdup(str);

  int i = 0;

  for (i = 0; i < level; i++) {
    indent_string[i] = ' ';
    indent_string[i * 2 + 1] = ' ';
  }

  while((token = strsep(&newstr, "\n")) != NULL) {
    printf("%s%s\n", indent_string, token);
  }

  free(tofree);
}

void p_section(char *header, char *usage, struct help_cols *fields, struct help_cols *out_opts) {
  help_opts *opts = HELP_ALLOC_OPTS;
  opts->indent = 1;

  p_header(header);

  p_indent(1, usage);

  if (fields) {
    p_header("Fields");
    p_cols(opts, fields);
  }

  if (out_opts) {
    p_header("Options");
    p_cols(opts, out_opts);
  }
}

void p_cols(help_opts *opts, struct help_cols *cols) {
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

void print_version() {
  printf("%s\n", VERSION);
}

void print_usage() {
  help_opts *opts = HELP_ALLOC_OPTS;
  opts->indent = 1;

  p_header(APPNAME);
  p_indent(1, "A tool for working with NES ROM files in the iNES v1 format");
  p_newline();

  p_header("Usage");
  p_indent(1, APPNAME " <subcommand> [ <subcommand-args> ... ]");
  p_newline();

  p_header("Subcommands");

  static struct help_cols subcommand_table[] = {
    { "help", "Print the usage for the given subcommand" },
    { "info", "Print an information dump about the given rom" },
    { "prg", "Perform PRG bank actions such as extraction and replacement" },
    { "chr", "Perform CHR bank actions such as extraction and replacement" },
    { "title", "Perform Title metadata actions such as reading and setting" },
    NULL_COL
  };

  p_cols(opts, subcommand_table);

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

  p_section(
    "Extraction",
    "nrt prg extract <rom> [ <options> ... ] <bank-index> <outfile>",
    extraction_fields,
    extraction_options
  );
}

void print_usage_chr() {
  help_opts *opts = HELP_ALLOC_OPTS;
  opts->indent = 1;

  p_header(APPNAME " chr");
  p_indent(1, "Perform CHR bank action such as extraction and replacement");
  p_newline();

  p_header("Usage");
  p_indent(1, APPNAME " chr <action> [ <action-args> ... ]");
  p_newline();

  p_header("Actions");

  static struct help_cols action_cols[] = {
    "extract", "Extract the given CHR to either a raw CHR bank or a PNG file",
    "replace", "Replace a CHR bank with the given PNG or raw CHR bank",
    NULL_COL
  };

  p_cols(opts, action_cols);

  p_newline();

  printf("For additional usage information for the above actions run `%s chr <action> --help`\n", APPNAME);

  p_newline();
}

void print_usage_chr_extract() {
  help_opts *opts = HELP_ALLOC_OPTS;
  opts->indent = 1;

  static struct help_cols option_cols[] = {
    "--help", "Display this help",
    "-f | --format <format>", "Export to the selected format: raw or png",
    "-w | --width <width>", "Applicable only for PNG output: The number of tiles wide the final image should be (default: 16)",
    NULL_COL
  };

  static struct help_cols arg_cols[] = {
    "<rom>", "The path to the source ROM file to extract from",
    "<bank-index>", "The zero-based index of the bank to extract",
    "<outflie>", "The output file.",
    NULL_COL
  };

  p_header(APPNAME " chr extract");
  p_indent(1,
      "Extract a CHR bank from a ROM file to either a raw CHR bank file or a PNG bitmap\n"
      "The format is determined by either the file extension of the output file or\n"
      "the value of the -f or --format flag"
  );
  p_newline();

  p_header("Usage");
  p_indent(1, APPNAME " chr extract <rom> [ <options> ... ] <bank-index> <outfile>");
  p_newline();

  p_header("Arguments");
  p_cols(opts, arg_cols);
  p_newline();

  p_header("Options");
  p_cols(opts, option_cols);
  p_newline();
}

void print_usage_chr_replace() {
  help_opts *opts = HELP_ALLOC_OPTS;
  opts->indent = 1;

  static struct help_cols option_cols[] = {
    "--help", "Display this help",
    "-f | --format <format>", "Export to the selected format: raw or png",
    "-o | --outfile <path>", "When replacing the bank, write the updated ROM file to this path rather than clobbering the source ROM",
    NULL_COL
  };

  static struct help_cols arg_cols[] = {
    "<rom>", "The path to the source ROM file to extract from",
    "<bank-index>", "The zero-based index of the bank to extract",
    "<chrbank>", "The source CHR bank file; either a raw CHR bank or a png bitmap",
    NULL_COL
  };

  p_header(APPNAME " chr replace");
  p_indent(1,
      "Replace a CHR bank in a ROM file with the given bank or png image."
  );
  p_newline();

  p_header("Usage");
  p_indent(1, APPNAME " chr replace <rom> [ <options> ... ] <bank-index> <chrbank>");
  p_newline();

  p_header("Arguments");
  p_cols(opts, arg_cols);
  p_newline();

  p_header("Options");
  p_cols(opts, option_cols);
  p_newline();
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

  p_cols(opts, subcommand_table);
}

