#ifndef _NESROMTOOL_PRG_H_
#define _NESROMTOOL_PRG_H_

#define NES_PRG_BANK_SIZE 16384
#define NES_PRG_BANK_MAX_COUNT 64

typedef struct nesprgbank_t {
  unsigned char data[NES_PRG_BANK_SIZE];
} nesprgbank_t;

#endif
