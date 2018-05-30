#ifndef _NRT_PRG_H_
#define _NRT_PRG_H_

#define NRT_PRG_BANK_SIZE 16384
#define NRT_PRG_BANK_MAX_COUNT 64

typedef struct nrt_prgbank_t {
  unsigned char data[NRT_PRG_BANK_SIZE];
} nrt_prgbank_t;

#endif
