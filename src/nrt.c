#include "nrt.h"

nrt_prgbank_list_t* nrt_prgbank_list_create(nrt_prgbank_t *prg, nrt_prgbank_list_t *next) {
  nrt_prgbank_list_t *list = NRT_PRG_LIST_ALLOC;
  list->prg = prg;
  list->next = next;

  return list;
}

nrt_prgbank_list_t* nrt_prgbank_list_prepend(nrt_prgbank_list_t *list, nrt_prgbank_t *prg) {
  return nrt_prgbank_list_insert(list, prg, 0);
}

nrt_prgbank_list_t* nrt_prgbank_list_append(nrt_prgbank_list_t *list, nrt_prgbank_t *prg) {
  int last_index = nrt_prgbank_list_count(list);

  return nrt_prgbank_list_insert(list, prg, last_index);
}

nrt_prgbank_list_t* nrt_prgbank_list_insert(nrt_prgbank_list_t *list, nrt_prgbank_t *prg, int index) {
  // initialize our new listitem.
  nrt_prgbank_list_t *new_list = nrt_prgbank_list_create(prg, NULL);

  // we're prepending, so this is super-simple
  if ( index == 0 ) {
    new_list->next = list;
    return new_list;
  }

  nrt_prgbank_list_t *before = nrt_prgbank_list_at(list, index - 1);
  nrt_prgbank_list_t *after = before->next;

  before->next = new_list;
  new_list->next = after;

  return list;
}

nrt_prgbank_list_t* nrt_prgbank_list_at(nrt_prgbank_list_t *list, int index) {
  nrt_prgbank_list_t *l = list;
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

nrt_prgbank_list_t* nrt_prgbank_list_last(nrt_prgbank_list_t *list) {
  nrt_prgbank_list_t *l;

  while (l) {
    if (!l->next) {
      return l;
    }

    l = l->next;
  }

  return NULL;
}

int nrt_prgbank_list_count(nrt_prgbank_list_t *list) {
  nrt_prgbank_list_t *l = list;
  int counter = 0;

  while (l != NULL) {
    counter++;

    l = l->next;
  }

  return counter;
}

// free the whole list and the associated PRG
void nrt_prgbank_list_free(nrt_prgbank_list_t *list) {
  nrt_prgbank_list_t *l = list;
  nrt_prgbank_list_t *next = l->next;

  while (l) {
    if (l->prg) {
      free(l->prg);
    }

    next = l->next;
    free(l);

    l = next;
  }
}
