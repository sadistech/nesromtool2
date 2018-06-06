#ifndef _NRT_H_
#define _NRT_H_

#include <stdbool.h>
#include "nrt/util.h"
#include "nrt/chr.h"
#include "nrt/prg.h"
#include "nrt/error.h"
#include "nrt/header.h"

#define NRT_PRG_LIST_ALLOC (nrt_prgbank_list_t*)malloc(sizeof(nrt_prgbank_list_t))
#define NRT_CHR_LIST_ALLOC (nrt_chrbank_list_t*)malloc(sizeof(nrt_chrbank_list_t))

typedef struct nrt_prgbank_list_t {
  nrt_prgbank_t *prg;
  struct nrt_prgbank_list_t *next;
} nrt_prgbank_list_t;

typedef struct nrt_chrbank_list_t {
  nrt_chrbank_t *chr;
  struct nrg_chrbank_list_t *next;
} nrt_chrbank_list_t;

typedef struct nrt_rom_t {
  nrt_header_t *header;
  nrt_prgbank_list_t *prg_banks;
  nrt_chrbank_list_t *chr_banks;
  char title[256];
} nrt_rom_t;

nrt_prgbank_list_t* nrt_prgbank_list_create(nrt_prgbank_t *prg, nrt_prgbank_list_t *next);
nrt_prgbank_list_t* nrt_prgbank_list_prepend(nrt_prgbank_list_t *list, nrt_prgbank_t *prg);
nrt_prgbank_list_t* nrt_prgbank_list_insert(nrt_prgbank_list_t *list, nrt_prgbank_t *prg, int index);
nrt_prgbank_list_t* nrt_prgbank_list_append(nrt_prgbank_list_t *list, nrt_prgbank_t *prg);
nrt_prgbank_list_t* nrt_prgbank_list_at(nrt_prgbank_list_t *list, int index);
nrt_prgbank_list_t* nrt_prgbank_list_last(nrt_prgbank_list_t *list);
int nrt_prgbank_list_count(nrt_prgbank_list_t *list);
void nrt_prgbank_list_free(nrt_prgbank_list_t *list);

#endif
