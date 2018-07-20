srcdir=./src
libsrcdir=$(srcdir)/nrt

builtdir=./bin

nrtbuilt=$(builtdir)/nrt
rombuilder=$(builtdir)/nrt_builder

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

all: $(nrtbuilt) $(rombuilder)

$(builtdir):
	mkdir $(builtdir)

$(nrtbuilt): ./bin $(srcfiles) $(headers) $(srcdir)/nrt_main.c
	gcc -o $(nrtbuilt) $(srcfiles) $(srcdir)/nrt_main.c -lpng

$(rombuilder): ./bin $(srcfiles) $(headers) $(srcdir)/rombuilder_main.c
	gcc -o $(rombuilder) $(srcfiles) $(srcdir)/rombuilder_main.c -lpng

bitmaptest: ./bin $(srcfiles) $(headers) $(srcdir)/bitmap_main.c
	gcc -g -o bin/bitmap $(srcfiles) $(srcdir)/bitmap_main.c -lpng

run: $(nrtbuilt) all
	$(nrtbuilt) verify ./mario3.nes

clean:
	rm -rf $(builtdir) $(builttest)

test: .PHONY
	./runtests.sh

.PHONY:
