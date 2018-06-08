#include "nrt.h"

nrt_prg_list* nrt_prg_list_create(void)
{
  nrt_prg_list* list = (nrt_prg_list*)malloc(sizeof(nrt_prg_list));
  list->count = 0;
  list->list = NULL;

  return list;
}

nrt_prg_listitem* nrt_prg_listitem_create(nrt_prgbank* prg, nrt_prg_listitem* next)
{
  nrt_prg_listitem *list = NRT_PRG_LISTITEM_ALLOC;
  list->prg = prg;
  list->next = next;

  return list;
}

nrt_prg_list* nrt_prg_list_prepend(nrt_prg_list* list, nrt_prgbank* prg)
{
  return nrt_prg_list_insert(list, prg, 0);
}

nrt_prg_list* nrt_prg_list_append(nrt_prg_list* list, nrt_prgbank *prg)
{
  return nrt_prg_list_insert(list, prg, list->count);
}

nrt_prg_list* nrt_prg_list_insert(nrt_prg_list* list, nrt_prgbank* prg, int index)
{
  // initialize our new listitem.
  nrt_prg_listitem *new_item = nrt_prg_listitem_create(prg, NULL);

  list->count++;

  // we're prepending, so this is super-simple
  if ( index == 0 ) {
    new_item->next = list->list;
    list->list = new_item;
    return list;
  }

  nrt_prg_listitem *before = nrt_prg_listitem_at(list, index - 1);
  nrt_prg_listitem *after = before->next;

  before->next = new_item;
  new_item->next = after;

  return list;
}

nrt_prg_listitem* nrt_prg_listitem_at(nrt_prg_list* list, int index)
{
  nrt_prg_listitem *l = list->list;
  int counter = 0;

  while (l) {
    if (counter == index) {
      return l;
    }

    counter++;
    l = l->next;
  }

  return NULL;
}

nrt_prg_listitem* nrt_prg_listitem_last(nrt_prg_list* list)
{
  nrt_prg_listitem *l = list->list;

  while (l) {
    if (!l->next) {
      return l;
    }

    l = l->next;
  }

  return NULL;
}

int nrt_prg_listitem_count(nrt_prg_listitem *list) {
  nrt_prg_listitem *l = list;
  int counter = 0;

  while (l != NULL) {
    counter++;

    l = l->next;
  }

  return counter;
}

// free the whole list and the associated PRG
void nrt_prg_list_free(nrt_prg_list *list) {
  nrt_prg_listitem *l = list->list;
  nrt_prg_listitem *next = l->next;

  while (l) {
    if (l->prg) {
      free(l->prg);
    }

    next = l->next;
    free(l);

    l = next;
  }

  free(list);
}

/*
nrt_chr_listitem* nrt_chr_listitem_create(nrt_chrbank *chr, nrt_chr_listitem *next) {
  nrt_chr_listitem *list = NRT_CHR_LIST_ALLOC;
  list->chr = chr;
  list->next = next;

  return list;
}

nrt_chr_listitem* nrt_chr_listitem_prepend(nrt_chr_listitem *list, nrt_chrbank *chr) {
  return nrt_chr_listitem_insert(list, chr, 0);
}

nrt_chr_listitem* nrt_chr_listitem_append(nrt_chr_listitem *list, nrt_chrbank *chr) {
  int last_index = nrt_chr_listitem_count(list);

  return nrt_chr_listitem_insert(list, chr, last_index);
}

nrt_chr_listitem* nrt_chr_listitem_insert(nrt_chr_listitem *list, nrt_chrbank *chr, int index) {
  // initialize our new listitem.
  nrt_chr_listitem *new_list = nrt_chr_listitem_create(chr, NULL);

  // we're prepending, so this is super-simple
  if ( index == 0 ) {
    new_list->next = list;
    return new_list;
  }

  nrt_chr_listitem *before = nrt_chr_listitem_at(list, index - 1);
  nrt_chr_listitem *after = before->next;

  before->next = new_list;
  new_list->next = after;

  return list;
}

nrt_chr_listitem* nrt_chr_listitem_at(nrt_chr_listitem *list, int index) {
  nrt_chr_listitem *l = list;
  int counter = 0;

  while (l) {
    if (counter == index) {
      return l;
    }

    counter++;
    l = l->next;
  }

  return NULL;
}

nrt_chr_listitem* nrt_chr_listitem_last(nrt_chr_listitem *list) {
  nrt_chr_listitem *l;

  while (l) {
    if (!l->next) {
      return l;
    }

    l = l->next;
  }

  return NULL;
}

int nrt_chr_listitem_count(nrt_chr_listitem *list) {
  nrt_chr_listitem *l = list;
  int counter = 0;

  while (l != NULL) {
    counter++;

    l = l->next;
  }

  return counter;
}

// free the whole list and the associated CHR
void nrt_chr_listitem_free(nrt_chr_listitem *list) {
  nrt_chr_listitem *l = list;
  nrt_chr_listitem *next = l->next;

  while (l) {
    if (l->chr) {
      free(l->chr);
    }

    next = l->next;
    free(l);

    l = next;
  }
}

nrt_rom* nrt_rom_create(void) {
  nrt_rom* rom = NRT_ROM_ALLOC;

  rom->header = NRT_HEADER_ALLOC;
  rom->prg_banks = NULL;
  rom->chr_banks = NULL;

  bzero(rom->title, NRT_TITLE_MAX_LENGTH);

  return rom;
}

void nrt_rom_free(nrt_rom* rom) {
  free(rom->header);
  nrt_prg_listitem_free(rom->prg_banks);
  nrt_chr_listitem_free(rom->chr_banks);
}

nrt_rom* nrt_read_rom_from_file(FILE* romfile) {
  nrt_rom* rom = nrt_rom_create();

  fseek(romfile, 0, SEEK_SET);

  if (nrt_header_extract(romfile, rom->header) != 1) {
    nrt_rom_free(rom);
    return NULL;
  }

  int i;
  for (i = 0; i < rom->header->prg_count; i++) {
    nrt_prgbank* prg = NRT_PRG_ALLOC;

    if (nrt_extract_prg(romfile, i, prg) != 1) {
      nrt_rom_free(rom);
      free(prg);
      return NULL;
    }

    if (i == 0) {
      // we need to start the prg stuff
      rom->prg_banks = nrt_prg_listitem_create(prg, NULL);
    } else {
      nrt_prg_listitem_append(rom->prg_banks, prg);
    }
  }

  for (i = 0; i < rom->header->chr_count; i++) {
    nrt_chrbank* chr = NRT_CHR_ALLOC;

    if (nrt_extract_chr(romfile, rom->header, i, chr) != 1) {
      nrt_rom_free(rom);
      free(chr);
      return NULL;
    }

    if (i == 0) {
      // we need to start the chr stuff
      rom->chr_banks = nrt_chr_listitem_create(chr, NULL);
    } else {
      nrt_chr_listitem_append(rom->chr_banks, chr);
    }
  }

  nrt_read_title_from_file(romfile, rom->header, rom->title);

  return rom;
}*/
