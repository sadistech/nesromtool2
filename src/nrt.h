#ifndef _NRT_H_
#define _NRT_H_

#include <stdbool.h>
#include "nrt/util.h"
#include "nrt/chr.h"
#include "nrt/prg.h"
#include "nrt/error.h"
#include "nrt/header.h"
#include "nrt/title.h"

#define NRT_PRG_LIST_ALLOC (nrt_prgbank_list*)malloc(sizeof(nrt_prgbank_list))
#define NRT_CHR_LIST_ALLOC (nrt_chrbank_list*)malloc(sizeof(nrt_chrbank_list))
#define NRT_ROM_ALLOC (nrt_rom*)malloc(sizeof(nrt_rom))

typedef struct nrt_prgbank_list {
  nrt_prgbank *prg;
  struct nrt_prgbank_list *next;
} nrt_prgbank_list;

typedef struct nrt_chrbank_list {
  nrt_chrbank *chr;
  struct nrt_chrbank_list *next;
} nrt_chrbank_list;

typedef struct nrt_rom {
  nrt_header *header;
  nrt_prgbank_list *prg_banks;
  nrt_chrbank_list *chr_banks;
  char title[NRT_TITLE_MAX_LENGTH];
} nrt_rom;

nrt_prgbank_list* nrt_prgbank_list_create(nrt_prgbank *prg, nrt_prgbank_list *next);
nrt_prgbank_list* nrt_prgbank_list_prepend(nrt_prgbank_list *list, nrt_prgbank *prg);
nrt_prgbank_list* nrt_prgbank_list_insert(nrt_prgbank_list *list, nrt_prgbank *prg, int index);
nrt_prgbank_list* nrt_prgbank_list_append(nrt_prgbank_list *list, nrt_prgbank *prg);
nrt_prgbank_list* nrt_prgbank_list_at(nrt_prgbank_list *list, int index);
nrt_prgbank_list* nrt_prgbank_list_last(nrt_prgbank_list *list);
int nrt_prgbank_list_count(nrt_prgbank_list *list);
void nrt_prgbank_list_free(nrt_prgbank_list *list);

nrt_chrbank_list* nrt_chrbank_list_create(nrt_chrbank *chr, nrt_chrbank_list *next);
nrt_chrbank_list* nrt_chrbank_list_prepend(nrt_chrbank_list *list, nrt_chrbank *chr);
nrt_chrbank_list* nrt_chrbank_list_insert(nrt_chrbank_list *list, nrt_chrbank *chr, int index);
nrt_chrbank_list* nrt_chrbank_list_append(nrt_chrbank_list *list, nrt_chrbank *chr);
nrt_chrbank_list* nrt_chrbank_list_at(nrt_chrbank_list *list, int index);
nrt_chrbank_list* nrt_chrbank_list_last(nrt_chrbank_list *list);
int nrt_chrbank_list_count(nrt_chrbank_list *list);
void nrt_chrbank_list_free(nrt_chrbank_list *list);

nrt_rom* nrt_rom_create(void);
void nrt_rom_free(nrt_rom* rom);
nrt_rom* nrt_read_rom_from_file(FILE* romfile);

#endif
