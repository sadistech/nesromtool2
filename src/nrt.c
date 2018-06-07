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
