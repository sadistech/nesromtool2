srcdir=./src
libsrcdir=$(srcdir)/nrt

builtdir=./bin

nrtbuilt=$(builtdir)/nrt
rombuilder=$(builtdir)/nrt_builder
chr2png=$(builtdir)/chr2png
png2chr=$(builtdir)/png2chr
tilec=$(builtdir)/tilec

CFLAGS=

srcfiles=\
		$(libsrcdir)/error.c \
		$(libsrcdir)/util.c \
		$(libsrcdir)/header.c \
		$(libsrcdir)/prg.c \
		$(libsrcdir)/tile.c \
		$(libsrcdir)/chr.c \
		$(libsrcdir)/title.c \
		$(srcdir)/nrt.c \
		$(srcdir)/util.c \
		$(srcdir)/help.c \
		$(srcdir)/subcommands.c \
		$(srcdir)/subcommands/info.c \
		$(srcdir)/subcommands/prg.c \
		$(srcdir)/subcommands/chr.c \
		$(srcdir)/subcommands/title.c

headers=\
		$(libsrcdir)/error.h \
		$(libsrcdir)/util.h \
		$(libsrcdir)/header.h \
		$(libsrcdir)/prg.h \
		$(libsrcdir)/tile.h \
		$(libsrcdir)/chr.h \
		$(libsrcdir)/title.h \
		$(srcdir)/nrt.h \
		$(srcdir)/util.h \
		$(srcdir)/help.h \
		$(srcdir)/subcommands.h \
		$(srcdir)/subcommands/info.h \
		$(srcdir)/subcommands/prg.h \
		$(srcdir)/subcommands/chr.h \
		$(srcdir)/subcommands/title.h


testfiles=$(srcdir)/test.c
testheaders=$(libsrcdir)/test.h
builttest="./test_nrt"

all: $(nrtbuilt) $(rombuilder) $(chr2png) $(png2chr) $(tilec)

$(builtdir):
	mkdir $(builtdir)

$(nrtbuilt): ./bin $(srcfiles) $(headers) $(srcdir)/nrt_main.c
	gcc -o $(nrtbuilt) $(srcfiles) $(srcdir)/nrt_main.c $(CFLAGS) -lpng -lm -lz

$(rombuilder): ./bin $(srcfiles) $(headers) $(srcdir)/rombuilder_main.c
	gcc -o $(rombuilder) $(srcfiles) $(srcdir)/rombuilder_main.c $(CFLAGS) -lpng -lm -lz

$(chr2png): ./bin $(srcfiles) $(headers) $(srcdir)/chr2png_main.c
	gcc -o $(chr2png) $(srcfiles) $(srcdir)/chr2png_main.c $(CFLAGS) -lpng -lm -lz

$(png2chr): ./bin $(srcfiles) $(headers) $(srcdir)/chr2png_main.c
	gcc -o $(png2chr) $(srcfiles) $(srcdir)/chr2png_main.c $(CFLAGS) -lpng -lm -lz

$(tilec): ./bin $(srcfiles) $(headers) $(srcdir)/tilec_main.c
	gcc -o $(tilec) $(srcfiles) $(srcdir)/tilec_main.c $(CFLAGS) -lpng -lm -lz

run: $(nrtbuilt) all
	$(nrtbuilt) verify ./mario3.nes

clean:
	rm -rf $(builtdir) $(builttest)

test: .PHONY
	./runtests.sh

.PHONY:
