#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <limits.h>
#include <getopt.h>

#include "nrt.h"
#include "util.h"
#include "help.h"

/*
 * usage:
 * chr2png [ --outdir=<outdir> --width=<width> ] <foo.chr> [ <bar.chr> ... ]
 * png2chr [ --outdir=<outdir> ] <foo.png> [ <bar.png> ... ]
 *
 * convert a chr file into a png with the same basename and vice versa
 */

typedef enum apptype {
  app_none,
  app_chr2png,
  app_png2chr
} apptype;

typedef struct options {
  apptype app;
  char outdir[PATH_MAX];
  int width;
} options;

void validate_outdir(char *outdir);
void validate_chrfile(char *filepath);
void validate_pngfile(char *filepath);

int chr2png(options *opts, int argc, char **argv);
int png2chr(options *opts, int argc, char **argv);

int main(int argc, char **argv) {
  options *opts = (options*)calloc(1, sizeof(options));

  char *appname = basename(argv[0]);

  if (strcmp(appname, "chr2png") == 0) {
    opts->app = app_chr2png;
    opts->width = 16; // default width;
  } else if (strcmp(appname, "png2chr") == 0) {
    opts->app = app_png2chr;
  } else {
    fprintf(stderr, "Cannot run program. Unknown argv[0] (%s)\n", appname);
    exit(EXIT_FAILURE);
  }

  // now let's parse arguments

  static struct option longopts[] = {
    { "help", no_argument, NULL, 'h' },
    { "outdir", required_argument, NULL, 'o' },
    { "width", required_argument, NULL, 'w' },
    { NULL, 0, NULL, 0 }
  };

  int ch;

  while ((ch = getopt_long(argc, argv, "ho:w:", longopts, NULL)) != -1) {
    switch (ch) {
      case 'h':
        if (opts->app == app_chr2png) {
          print_usage_chr2png();
        } else {
          print_usage_png2chr();
        }
        exit(EXIT_SUCCESS);
        break;

      case 'o':
        strcpy(opts->outdir, optarg);
        break;

      case 'w':
        if (opts->app == app_png2chr) {
          fprintf(stderr, "Setting width has no effect when converting to CHR.\n");
        }

        opts->width = atoi(optarg);
        break;

      case 0:
        break;
      default:
        fprintf(stderr, "Invalid argument: -%c\n", ch);
        exit(EXIT_FAILURE);
    }
  }

  argc -= optind;
  argv += optind;

  switch (opts->app) {
    case app_chr2png:
      chr2png(opts, argc, argv);
      break;
    case app_png2chr:
      png2chr(opts, argc, argv);
      break;
    default:
      printf("Invalid argument.\n");
      exit(EXIT_FAILURE);
  }
}

void validate_outdir(char *outdir) {
  // first, let's make sure the opts->outdir exists and is a directory before we get started
  // (assuming that it's specified)
  if (outdir[0] != '\0') {
    struct stat *outdir_stat = (struct stat*)malloc(sizeof(struct stat));

    if (stat(outdir, outdir_stat) != 0) {
      free(outdir_stat);
      perror(outdir);
      exit(EXIT_FAILURE);
    }

    if (!(outdir_stat->st_mode & S_IFDIR)) {
      free(outdir_stat);
      fprintf(stderr, "Output directory must be a directory.\n");
      exit(EXIT_FAILURE);
    }

    free(outdir_stat);
  }
}

void validate_chrfile(char *filepath) {
  struct stat *filestat = (struct stat*)malloc(sizeof(struct stat));

  if (stat(filepath, filestat) != 0) {
    free(filestat);
    perror(filepath);
    exit(EXIT_FAILURE);
  }

  if (!(filestat->st_mode & S_IFREG)) {
    free(filestat);
    fprintf(stderr, "CHR file must be a regular file.\n");
    exit(EXIT_FAILURE);
  }

  if (!nrt_chr_valid_filesize(filestat)) {
      fprintf(stderr, "CHR file does not appear to be a CHR file (incorrect filesize: %lld bytes; must be exactly %d bytes)\n", filestat->st_size, NRT_CHR_BANK_SIZE);
    free(filestat);
    exit(EXIT_FAILURE);
  }
}

void validate_pngfile(char *filepath) {
  struct stat *filestat = (struct stat*)malloc(sizeof(struct stat));

  if (stat(filepath, filestat) != 0) {
    free(filestat);
    perror(filepath);
    exit(EXIT_FAILURE);
  }

  if (!(filestat->st_mode & S_IFREG)) {
    free(filestat);
    fprintf(stderr, "CHR file must be a regular file.\n");
    exit(EXIT_FAILURE);
  }

  free(filestat);
}

int chr2png(options *opts, int argc, char **argv) {
  char *filename = argv[0];
  char output_path[PATH_MAX];

  validate_outdir(opts->outdir);

  while (filename) {
    validate_chrfile(filename);
    char *base_filename = filename_no_ext(basename(filename));

    // first let's figure out the to and from stuff.
    if (opts->outdir[0] == '\0') {
      // if the outdir is not specified, it's gonna be outpu to the same place as the original file
      // so to get the output path for each file,
      // we need to get the filename sans extension
      // and the dirname
      // and then concat dirname, basename, new extension
      sprintf(output_path, "%s/%s.png", dirname(filename), base_filename);
    } else {
      // the output path is set to somethign
      sprintf(output_path, "%s/%s.png", opts->outdir, base_filename);
    }

    printf("Will convert %s -> %s\n", filename, output_path);

    // read in chr from filesystem
    // convert to bitmap
    // write to tempfile
    // move tempfile in place.

    FILE *chrfile = fopen(filename, "r");

    if (!chrfile) {
      perror(filename);
      exit(EXIT_FAILURE);
    }

    nrt_chrbank *chr = (nrt_chrbank*)malloc(sizeof(nrt_chrbank));

    if (fread(chr, NRT_CHR_BANK_SIZE, 1, chrfile) != 1) {
      free(chr);
      fclose(chrfile);
      fprintf(stderr, "Failed to read CHR bank due to an unknown error.\n");
      exit(EXIT_FAILURE);
    }

    fclose(chrfile);

    nrt_tile_bitmap *bitmaps = (nrt_tile_bitmap*)malloc(NRT_CHR_TILE_COUNT * sizeof(nrt_tile_bitmap));
    int i;
    for (i = 0; i < NRT_CHR_TILE_COUNT; i++) {
      nrt_tile_to_bitmap(&chr->tile[i], &bitmaps[i]);
    }

    free(chr);

    FILE *outfile = fopen(output_path, "w");

    if (!outfile) {
      free(bitmaps);
      perror(output_path);
      exit(EXIT_FAILURE);
    }

    nrt_tiles_to_png(bitmaps, NRT_CHR_TILE_COUNT, opts->width, outfile);

    fclose(outfile);
    free(bitmaps);

    // get next and continue loop.
    filename = *(++argv);
  }

  return 0;
}

int png2chr(options *opts, int argc, char **argv) {
  char *filename = argv[0];
  char output_path[PATH_MAX];

  validate_outdir(opts->outdir);

  while (filename) {
    validate_pngfile(filename);
    char *base_filename = filename_no_ext(basename(filename));

    // first let's figure out the to and from stuff.
    if (opts->outdir[0] == '\0') {
      // if the outdir is not specified, it's gonna be outpu to the same place as the original file
      // so to get the output path for each file,
      // we need to get the filename sans extension
      // and the dirname
      // and then concat dirname, basename, new extension
      sprintf(output_path, "%s/%s.chr", dirname(filename), base_filename);
    } else {
      // the output path is set to somethign
      sprintf(output_path, "%s/%s.chr", opts->outdir, base_filename);
    }

    printf("Will convert %s -> %s\n", filename, output_path);

    // read in png to bitmap
    // convert to chr
    // write out to file

    FILE *pngfile = fopen(filename, "r");

    if (!pngfile) {
      perror(filename);
      exit(EXIT_FAILURE);
    }

    nrt_tile_bitmap *tiles = (nrt_tile_bitmap*)calloc(NRT_CHR_TILE_COUNT, sizeof(nrt_tile_bitmap));

    if (!nrt_png_to_tiles(pngfile, tiles)) {
      fprintf(stderr, "Failed to read PNG file: %s\n", filename);
      fclose(pngfile);
      exit(EXIT_FAILURE);
    }

    fclose(pngfile);

    nrt_chrbank *chr_bank = NRT_CHR_ALLOC;
    nrt_tile *raw_tile = NRT_TILE_ALLOC;

    // convert the bitmap into CHR stuff
    int i;
    for (i = 0; i < NRT_CHR_TILE_COUNT; i++) {
      nrt_bitmap_to_tile(&tiles[i], raw_tile);
      memcpy(&chr_bank->tile[i], raw_tile, NRT_TILE_SIZE);
    }

    free(raw_tile);
    free(tiles);

    // write this out.
    FILE *outfile = fopen(output_path, "w");

    if (!outfile) {
      free(chr_bank);
      perror(output_path);
      exit(EXIT_FAILURE);
    }

    if (fwrite(chr_bank, NRT_CHR_BANK_SIZE, 1, outfile) != 1) {
      free(chr_bank);
      fclose(outfile);
      perror(output_path);
      exit(EXIT_FAILURE);
    }

    fclose(outfile);
    free(chr_bank);

    // get next and continue loop.
    filename = *(++argv);
  }

  return 0;
}

