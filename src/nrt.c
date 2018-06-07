#include "nrt.h"

nrt_prgbank_list* nrt_prgbank_list_create(nrt_prgbank *prg, nrt_prgbank_list *next) {
  nrt_prgbank_list *list = NRT_PRG_LIST_ALLOC;
  list->prg = prg;
  list->next = next;

  return list;
}

nrt_prgbank_list* nrt_prgbank_list_prepend(nrt_prgbank_list *list, nrt_prgbank *prg) {
  return nrt_prgbank_list_insert(list, prg, 0);
}

nrt_prgbank_list* nrt_prgbank_list_append(nrt_prgbank_list *list, nrt_prgbank *prg) {
  int last_index = nrt_prgbank_list_count(list);

  return nrt_prgbank_list_insert(list, prg, last_index);
}

nrt_prgbank_list* nrt_prgbank_list_insert(nrt_prgbank_list *list, nrt_prgbank *prg, int index) {
  // initialize our new listitem.
  nrt_prgbank_list *new_list = nrt_prgbank_list_create(prg, NULL);

  // we're prepending, so this is super-simple
  if ( index == 0 ) {
    new_list->next = list;
    return new_list;
  }

  nrt_prgbank_list *before = nrt_prgbank_list_at(list, index - 1);
  nrt_prgbank_list *after = before->next;

  before->next = new_list;
  new_list->next = after;

  return list;
}

nrt_prgbank_list* nrt_prgbank_list_at(nrt_prgbank_list *list, int index) {
  nrt_prgbank_list *l = list;
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

nrt_prgbank_list* nrt_prgbank_list_last(nrt_prgbank_list *list) {
  nrt_prgbank_list *l;

  while (l) {
    if (!l->next) {
      return l;
    }

    l = l->next;
  }

  return NULL;
}

int nrt_prgbank_list_count(nrt_prgbank_list *list) {
  nrt_prgbank_list *l = list;
  int counter = 0;

  while (l != NULL) {
    counter++;

    l = l->next;
  }

  return counter;
}

// free the whole list and the associated PRG
void nrt_prgbank_list_free(nrt_prgbank_list *list) {
  nrt_prgbank_list *l = list;
  nrt_prgbank_list *next = l->next;

  while (l) {
    if (l->prg) {
      free(l->prg);
    }

    next = l->next;
    free(l);

    l = next;
  }
}

nrt_chrbank_list* nrt_chrbank_list_create(nrt_chrbank *chr, nrt_chrbank_list *next) {
  nrt_chrbank_list *list = NRT_CHR_LIST_ALLOC;
  list->chr = chr;
  list->next = next;

  return list;
}

nrt_chrbank_list* nrt_chrbank_list_prepend(nrt_chrbank_list *list, nrt_chrbank *chr) {
  return nrt_chrbank_list_insert(list, chr, 0);
}

nrt_chrbank_list* nrt_chrbank_list_append(nrt_chrbank_list *list, nrt_chrbank *chr) {
  int last_index = nrt_chrbank_list_count(list);

  return nrt_chrbank_list_insert(list, chr, last_index);
}

nrt_chrbank_list* nrt_chrbank_list_insert(nrt_chrbank_list *list, nrt_chrbank *chr, int index) {
  // initialize our new listitem.
  nrt_chrbank_list *new_list = nrt_chrbank_list_create(chr, NULL);

  // we're prepending, so this is super-simple
  if ( index == 0 ) {
    new_list->next = list;
    return new_list;
  }

  nrt_chrbank_list *before = nrt_chrbank_list_at(list, index - 1);
  nrt_chrbank_list *after = before->next;

  before->next = new_list;
  new_list->next = after;

  return list;
}

nrt_chrbank_list* nrt_chrbank_list_at(nrt_chrbank_list *list, int index) {
  nrt_chrbank_list *l = list;
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

nrt_chrbank_list* nrt_chrbank_list_last(nrt_chrbank_list *list) {
  nrt_chrbank_list *l;

  while (l) {
    if (!l->next) {
      return l;
    }

    l = l->next;
  }

  return NULL;
}

int nrt_chrbank_list_count(nrt_chrbank_list *list) {
  nrt_chrbank_list *l = list;
  int counter = 0;

  while (l != NULL) {
    counter++;

    l = l->next;
  }

  return counter;
}

// free the whole list and the associated CHR
void nrt_chrbank_list_free(nrt_chrbank_list *list) {
  nrt_chrbank_list *l = list;
  nrt_chrbank_list *next = l->next;

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
  nrt_prgbank_list_free(rom->prg_banks);
  nrt_chrbank_list_free(rom->chr_banks);
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
      rom->prg_banks = nrt_prgbank_list_create(prg, NULL);
    } else {
      nrt_prgbank_list_append(rom->prg_banks, prg);
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
      rom->chr_banks = nrt_chrbank_list_create(chr, NULL);
    } else {
      nrt_chrbank_list_append(rom->chr_banks, chr);
    }
  }

  nrt_read_title_from_file(romfile, rom->header, rom->title);

  return rom;
}
