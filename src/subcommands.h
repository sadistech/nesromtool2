#ifndef _SUBCOMMANDS_H_
#define _SUBCOMMANDS_H_

#define SUBCOMMAND(CMD, ACTION) \
  subcommand_ ## CMD ## _ ## ACTION( \
    subcommand_ ## CMD ## _ ## ACTION ## _parse(argc - 1, argv) \
  )

#include "util.h"

#include "subcommands/chr.h"
#include "subcommands/title.h"

#endif
