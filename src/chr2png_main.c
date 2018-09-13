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
  int size; /* expected number of tiles in the PNG file */
} options;

void validate_outdir(char *outdir);
void validate_chrfile(char *filepath);
void validate_pngfile(char *filepath);
int chr_tile_count(char *chrfile);

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
    { "size", required_argument, NULL, 's' },
    { NULL, 0, NULL, 0 }
  };

  int ch;

  while ((ch = getopt_long(argc, argv, "ho:w:s:", longopts, NULL)) != -1) {
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

      case 's':
        // if the value is 'chr' then that's 512
        // if the value is 'table' then it's 256
        // otherwise it should be a number
        if (strncmp(optarg, "chr", 4) == 0) {
          opts->size = 512;
        } else if (strncmp(optarg, "table", 6) == 0) {
          opts->size = 256;
        } else {
          opts->size = atoi(optarg);

          if (opts->size < 0 || opts->size > 512) {
            fprintf(stderr, "Please choose a value between 0 and 512\n");
            exit(EXIT_FAILURE);
          }
        }

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

  if (opts->size == 0) {
    opts->size = 512;
  }

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

int chr_tile_count(char *chrfile) {
  struct stat *chrfile_stat = (struct stat*)malloc(sizeof(struct stat));

  if (stat(chrfile, chrfile_stat) != 0) {
    free(chrfile_stat);
    perror(chrfile);
    exit(EXIT_FAILURE);
  }

  int size = chrfile_stat->st_size;
  free(chrfile_stat);

  // the file must be a size that's a multiple of 16 (the size of a tile in CHR memory)
  if (size % NRT_TILE_SIZE != 0) {
    fprintf(stderr, "File is the wrong size. It is %d bytes and must be a multiple of %d.\n", size, NRT_TILE_SIZE);
    exit(EXIT_FAILURE);
  }

  return size / NRT_TILE_SIZE;
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

    nrt_chrbank *chr = NRT_CHR_ALLOC;
    /* nrt_chrbank *chr = (nrt_chrbank*)malloc(sizeof(nrt_chrbank)); */

    // find out how many tiles are in this chr
    // only read that much
    int tile_count = chr_tile_count(filename);

    if (fread(chr, NRT_TILE_SIZE, tile_count, chrfile) != tile_count) {
      free(chr);
      fclose(chrfile);
      fprintf(stderr, "Failed to read CHR bank due to an unknown error.\n");
      exit(EXIT_FAILURE);
    }

    fclose(chrfile);

    nrt_tile_bitmap *bitmaps = (nrt_tile_bitmap*)calloc(tile_count, sizeof(nrt_tile_bitmap));
    int i;
    for (i = 0; i < tile_count; i++) {
      nrt_tile_to_bitmap(&chr->tile[i], &bitmaps[i]);
    }

    free(chr);

    FILE *outfile = fopen(output_path, "w");

    if (!outfile) {
      free(bitmaps);
      perror(output_path);
      exit(EXIT_FAILURE);
    }

    nrt_tiles_to_png(bitmaps, tile_count, opts->width, outfile);

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

    nrt_tile_bitmap_lockup *lockup = (nrt_tile_bitmap_lockup*)calloc(opts->size, sizeof(nrt_tile_bitmap_lockup));

    if (!nrt_png_to_tiles(pngfile, lockup)) {
      fprintf(stderr, "Failed to read PNG file: %s\n", filename);
      fclose(pngfile);
      exit(EXIT_FAILURE);
    }

    fclose(pngfile);

    if (lockup->count > opts->size) {
      fprintf(stderr, "Incoming image is larger than the expected size (%d > %d tiles)\n", lockup->count, opts->size);
      free(lockup->bitmaps);
      free(lockup);
      exit(EXIT_FAILURE);
    }

    nrt_chrbank *chr_bank = NRT_CHR_ALLOC;
    nrt_tile *raw_tile = NRT_TILE_ALLOC;

    // convert the bitmap into CHR stuff
    int i;
    for (i = 0; i < lockup->count; i++) {
      nrt_bitmap_to_tile(&lockup->bitmaps[i], raw_tile);
      memcpy(&chr_bank->tile[i], raw_tile, NRT_TILE_SIZE);
    }

    free(raw_tile);
    free(lockup->bitmaps);
    free(lockup);

    // write this out.
    FILE *outfile = fopen(output_path, "w");

    if (!outfile) {
      free(chr_bank);
      perror(output_path);
      exit(EXIT_FAILURE);
    }

    if (fwrite(chr_bank, NRT_TILE_SIZE, opts->size, outfile) != opts->size) {
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

