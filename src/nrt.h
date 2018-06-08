#ifndef _NRT_H_
#define _NRT_H_

#include <stdbool.h>
#include "nrt/util.h"
#include "nrt/chr.h"
#include "nrt/prg.h"
#include "nrt/error.h"
#include "nrt/header.h"
#include "nrt/title.h"

#define NRT_PRG_LISTITEM_ALLOC (nrt_prg_listitem*)malloc(sizeof(nrt_prg_listitem))
#define NRT_CHR_LISTITEM_ALLOC (nrt_chr_listitem*)malloc(sizeof(nrt_chr_listitem))
#define NRT_ROM_ALLOC (nrt_rom*)malloc(sizeof(nrt_rom))

typedef struct nrt_prg_listitem {
  nrt_prgbank* prg;
  struct nrt_prg_listitem *next;
} nrt_prg_listitem;

typedef struct nrt_chr_listitem {
  nrt_chrbank* chr;
  struct nrt_chr_listitem *next;
} nrt_chr_listitem;

typedef struct nrt_prg_list {
  int count;
  nrt_prg_listitem* list; // head
} nrt_prg_list;

typedef struct nrt_chr_list {
  int count;
  nrt_chr_listitem* list; // head
} nrt_chr_list;

typedef struct nrt_rom {
  nrt_header* header;
  nrt_prg_list* prgs;
  nrt_chr_list* chrs;
  char title[NRT_TITLE_MAX_LENGTH];
} nrt_rom;


nrt_prg_list* nrt_prg_list_create(void);
nrt_prg_listitem* nrt_prg_listitem_create(nrt_prgbank *prg, nrt_prg_listitem *next);

nrt_prg_list* nrt_prg_list_prepend(nrt_prg_list* list, nrt_prgbank* prg);
nrt_prg_list* nrt_prg_list_insert(nrt_prg_list* list, nrt_prgbank* prg, int index);
nrt_prg_list* nrt_prg_list_append(nrt_prg_list* list, nrt_prgbank* prg);
nrt_prg_listitem* nrt_prg_list_delete(nrt_prg_list* list, int index);
nrt_prg_listitem* nrt_prg_listitem_at(nrt_prg_list* list, int index);
nrt_prg_listitem* nrt_prg_listitem_last(nrt_prg_list* list);
int nrt_prg_listitem_count(nrt_prg_listitem* list);
void nrt_prg_list_free(nrt_prg_list* list);

nrt_chr_list* nrt_chr_list_create(void);
nrt_chr_listitem* nrt_chr_listitem_create(nrt_chrbank *chr, nrt_chr_listitem *next);

nrt_chr_list* nrt_chr_list_prepend(nrt_chr_list* list, nrt_chrbank* chr);
nrt_chr_list* nrt_chr_list_insert(nrt_chr_list* list, nrt_chrbank* chr, int index);
nrt_chr_list* nrt_chr_list_append(nrt_chr_list* list, nrt_chrbank* chr);
nrt_chr_listitem* nrt_chr_listitem_at(nrt_chr_list* list, int index);
nrt_chr_listitem* nrt_chr_listitem_last(nrt_chr_list* list);
int nrt_chr_listitem_count(nrt_chr_listitem* list);
void nrt_chr_list_free(nrt_chr_list* list);


nrt_rom* nrt_rom_create(void);
void nrt_rom_free(nrt_rom* rom);
nrt_rom* nrt_read_rom_from_file(FILE* romfile);

#endif
