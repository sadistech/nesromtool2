#include "chr.h"

void subcommand_chr(int argc, char **argv) {
  char *action = *(argv++);

  if (strcmp(action, "extract") == 0) {
    subcommand_chr_extract(argc - 1, argv);
  } else if (strcmp(action, "replace") == 0) {
    subcommand_chr_replace(argc - 1, argv);
  }
}

/*
 * Usage:
 * nrt chr extract <file> [ <options> ] <bank> <outfile>
 * options:
 *   --format=[png,raw]
 */
void subcommand_chr_extract(int argc, char **argv) {
  static struct option longopts[] = {
    { "format", required_argument, NULL, 'f' },
    { NULL, 0, NULL, 0 }
  };

  char format[32] = "";
  char outfile_path[1024] = "";
  char *bank_index_str = NULL;
  int bank_index = 0;
  char *infile_path = *(argv++);
  argc++;

  int ch;

  printf("pre-opt\n");
  while ((ch = getopt_long(argc, argv, "f:", longopts, NULL)) != -1) {
    switch (ch) {
      case 'f':
        strcpy(format, optarg);
        break;
      case 0:
        printf("nothing\n");
        break;
      default:
        fprintf(stderr, "Invalid argument: -%c\n", ch);
        exit(EXIT_FAILURE);
    }
  }

  printf("heh\n");

  argc -= optind;
  argv += optind;

  printf("parsed.\n");

  bank_index_str = *(argv++);
  strcpy(outfile_path, *(argv++));

  printf("read args\n");

  bank_index = atoi(bank_index_str);

  printf("Going to open %s and extract bank %d to '%s'\n", infile_path, bank_index, outfile_path);
  printf("Format: '%s'\n", format);
}

void subcommand_chr_replace(int argc, char **argv) {
  fprintf(stderr, "Not yet implemented.\n");
  exit(EXIT_FAILURE);
}
