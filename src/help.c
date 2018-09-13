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

  p_header(APPNAME " info");
  p_indent(1,
      "Print verbose information about the given ROM file including whether it's valid,\n"
      "the number of PRG and CHR banks, region and mapper information as well as title metadata."
  );
  p_newline();

  p_header("Usage");
  p_indent(1, APPNAME " info <rom>");
  p_newline();
}

void print_usage_prg() {
  help_opts *opts = HELP_ALLOC_OPTS;
  opts->indent = 1;

  p_header(APPNAME " prg");
  p_indent(1, "Perform PRG bank action such as extraction and replacement");
  p_newline();

  p_header("Usage");
  p_indent(1, APPNAME " prg <action> [ <action-args> ... ]");
  p_newline();

  p_header("Actions");

  static struct help_cols action_cols[] = {
    "extract", "Extract the given PRG bank index to a raw PRG bank",
    "replace", "Replace a PRG bank index with the given PRG bank",
    NULL_COL
  };

  p_cols(opts, action_cols);

  p_newline();

  printf("For additional usage information for the above actions run `%s prg <action> --help`\n", APPNAME);

  p_newline();
}

void print_usage_prg_extract() {
  help_opts *opts = HELP_ALLOC_OPTS;
  opts->indent = 1;

  static struct help_cols option_cols[] = {
    "--help", "Display this help",
    NULL_COL
  };

  static struct help_cols arg_cols[] = {
    "<rom>", "The path to the source ROM file to extract from",
    "<bank-index>", "The zero-based index of the PRG bank to extract",
    "<outflie>", "The output file.",
    NULL_COL
  };

  p_header(APPNAME " prg extract");
  p_indent(1,
    "Extract a PRG bank to a separate file."
  );
  p_newline();

  p_header("Usage");
  p_indent(1, APPNAME " prg extract <rom> [ <options> ... ] <bank-index> <outfile>");
  p_newline();

  p_header("Arguments");
  p_cols(opts, arg_cols);
  p_newline();

  p_header("Options");
  p_cols(opts, option_cols);
  p_newline();
}

void print_usage_prg_replace() {
  help_opts *opts = HELP_ALLOC_OPTS;
  opts->indent = 1;

  static struct help_cols option_cols[] = {
    "--help", "Display this help",
    "-o | --outfile <path>", "When replacing the bank, write the updated ROM file to this path rather than clobbering the source ROM",
    NULL_COL
  };

  static struct help_cols arg_cols[] = {
    "<rom>", "The path to the source ROM file to extract from",
    "<bank-index>", "The zero-based index of the bank to replace",
    "<prgbank>", "The source PRG bank file; a raw PRG bank",
    NULL_COL
  };

  p_header(APPNAME " prg replace");
  p_indent(1,
      "Replace a PRG bank in a ROM file with the given bank file"
  );
  p_newline();

  p_header("Usage");
  p_indent(1, APPNAME " prg replace <rom> [ <options> ... ] <bank-index> <prgbank>");
  p_newline();

  p_header("Arguments");
  p_cols(opts, arg_cols);
  p_newline();

  p_header("Options");
  p_cols(opts, option_cols);
  p_newline();
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
    { "extract", "Extract the given CHR to either a raw CHR bank or a PNG file" },
    { "replace", "Replace a CHR bank with the given PNG or raw CHR bank" },
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
    "<bank-index>", "The zero-based index of the CHR bank to extract",
    "<outflie>", "The output file",
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
    "-f | --format <format>", "Import from the selected format: raw or png; If not passed, the format will be guessed from the file extension of the sourcefile",
    "-o | --outfile <path>", "When replacing the bank, write the updated ROM file to this path rather than clobbering the source ROM",
    NULL_COL
  };

  static struct help_cols arg_cols[] = {
    "<rom>", "The path to the source ROM file to replace the CHR bank",
    "<bank-index>", "The zero-based index of the bank to replace",
    "<sourcefile>", "The source CHR bank file; either a raw CHR bank or a png bitmap",
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

  p_header("Notes");
  p_indent(1,
      "When importing a PNG file, the file MUST contain 256 8x8 pixel tiles and\n"
      "be in indexed color mode. Only the colors 0-3 of the file's pallete will\n"
      "be used when importing into the ROM."
  );
}

void print_usage_title() {
  help_opts *opts = HELP_ALLOC_OPTS;
  opts->indent = 1;

  p_header(APPNAME " title");
  p_indent(1,
      "Perform actions related to the title metadata in an NES ROM such as setting,\n"
      "getting and removing the metadata. Title metadata is stored at the end of\n"
      "the ROM file and is not consistently supported by emulators, however should\n"
      "not affect the playability of the file itself."
  );
  p_newline();

  p_header("Usage");
  p_indent(1, APPNAME " title <action> [ <action-args> ... ]");
  p_newline();

  p_header("Actions");

  static struct help_cols action_cols[] = {
    { "get", "Read the title metadata from the given ROM and output to STDOUT" },
    { "set", "Set the title to the provided string value" },
    { "remove", "Remove the title metadata entirely from the given ROM file" },
    NULL_COL
  };

  p_cols(opts, action_cols);

  p_newline();

  printf("For additional usage information for the above actions run `%s title <action> --help`\n", APPNAME);

  p_newline();
}

void print_usage_title_get() {
  help_opts *opts = HELP_ALLOC_OPTS;
  opts->indent = 1;

  static struct help_cols option_cols[] = {
    "--help", "Display this help",
    NULL_COL
  };

  static struct help_cols arg_cols[] = {
    "<rom>", "The path to the source ROM file to read the title",
    NULL_COL
  };

  p_header(APPNAME " title get");
  p_indent(1,
      "Read the title metadata (if there is any) and output to STDOUT.\n"
      "If there is no title metadata, nothing will be output."
  );
  p_newline();

  p_header("Usage");
  p_indent(1, APPNAME " title get <rom>");
  p_newline();

  p_header("Arguments");
  p_cols(opts, arg_cols);
  p_newline();

  p_header("Options");
  p_cols(opts, option_cols);
  p_newline();
}

void print_usage_title_set() {
  help_opts *opts = HELP_ALLOC_OPTS;
  opts->indent = 1;

  static struct help_cols option_cols[] = {
    "--help", "Display this help",
    NULL_COL
  };

  static struct help_cols arg_cols[] = {
    "<rom>", "The path to the ROM file to set the title on",
    "<title>", "The new title to set (max: 256 chars)",
    NULL_COL
  };

  p_header(APPNAME " title set");
  p_indent(1,
      "Set the title metadata on the given ROM. Titles can be a maximum of 256 chars.\n"
      "If the title is an empty string, then remove the title entirely."
  );
  p_newline();

  p_header("Usage");
  p_indent(1, APPNAME " title set <rom> <title>");
  p_newline();

  p_header("Arguments");
  p_cols(opts, arg_cols);
  p_newline();

  p_header("Options");
  p_cols(opts, option_cols);
  p_newline();
}

void print_usage_title_remove() {
  help_opts *opts = HELP_ALLOC_OPTS;
  opts->indent = 1;

  static struct help_cols option_cols[] = {
    "--help", "Display this help",
    NULL_COL
  };

  static struct help_cols arg_cols[] = {
    "<rom>", "The path to the source ROM file to remove the title",
    NULL_COL
  };

  p_header(APPNAME " title remove");
  p_indent(1,
      "Remove the title metadata from the given ROM file."
  );
  p_newline();

  p_header("Usage");
  p_indent(1, APPNAME " title remove <rom>");
  p_newline();

  p_header("Arguments");
  p_cols(opts, arg_cols);
  p_newline();

  p_header("Options");
  p_cols(opts, option_cols);
  p_newline();
}

void print_usage_chr2png() {
  help_opts *opts = HELP_ALLOC_OPTS;
  opts->indent = 1;

  static struct help_cols option_cols[] = {
    "--help", "Display this help",
    "-w | --width=<width>", "Set the width (in tiles) of the output PNG",
    "-o | --outdir=<outdir>", "Set the output directory for the output PNG files (default is to output to same directory as source file)",
    NULL_COL
  };

  static struct help_cols arg_cols[] = {
    "<chrfile>", "A raw CHR bank file; possibly exported from a ROM, for example",
    NULL_COL
  };

  p_header("chr2png");
  p_indent(1,
      "Convert stand-alone CHR bank files into indexed 4-color raster PNG files"
  );
  p_newline();

  p_header("Usage");
  p_indent(1, "chr2png [ <options> ... ] <chrfile> [ <chrfile> ... ]");
  p_newline();

  p_header("Arguments");
  p_cols(opts, arg_cols);
  p_newline();

  p_header("Options");
  p_cols(opts, option_cols);
  p_newline();
}

void print_usage_png2chr() {
  help_opts *opts = HELP_ALLOC_OPTS;
  opts->indent = 1;

  static struct help_cols option_cols[] = {
    "--help", "Display this help",
    "-o | --outdir=<outdir>", "Set the output directory for the output PNG files (default is to output to same directory as source file)",
    "-s | --size=<size>", "Set the number of tiles in the target CHR file. This can be an integer value (from 1 up to 512) or 'table' for 256 or 'chr' for 512.",
    NULL_COL
  };

  static struct help_cols arg_cols[] = {
    "<pngfile>", "An indexed color raster PNG file",
    NULL_COL
  };

  p_header("png2chr");
  p_indent(1,
      "Convert raster PNG files into raw CHR banks. Input files must be indexed color mode\n"
      "and only the first 4 colors in the palette will be used"
  );
  p_newline();

  p_header("Usage");
  p_indent(1, "png2chr [ <options> ... ] <pngfile> [ <pngfile> ... ]");
  p_newline();

  p_header("Arguments");
  p_cols(opts, arg_cols);
  p_newline();

  p_header("Options");
  p_cols(opts, option_cols);
  p_newline();
}
