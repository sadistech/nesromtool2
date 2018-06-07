srcdir=./src
libsrcdir=$(srcdir)/nrt

builtdir=./bin
builtfile=$(builtdir)/nrt

srcfiles=$(libsrcdir)/error.c $(libsrcdir)/util.c $(libsrcdir)/header.c $(libsrcdir)/prg.c $(libsrcdir)/tile.c $(libsrcdir)/chr.c $(srcdir)/nrt.c
headers=$(libsrcdir)/error.h $(libsrcdir)/util.h $(libsrcdir)/header.h $(libsrcdir)/prg.h $(libsrcdir)/tile.h $(libsrcdir)/chr.h $(srcdir)/nrt.h

testfiles=$(srcdir)/test.c
testheaders=$(libsrcdir)/test.h
builttest="./test_nrt"

all: $(builtfile)

$(builtdir):
	mkdir $(builtdir)

$(builtfile): ./bin $(srcfiles) $(headers)
	gcc -o $(builtfile) $(srcfiles) $(srcdir)/nrt_main.c 

run: $(builtfile) all
	$(builtfile) verify ./mario3.nes

clean:
	rm -rf $(builtdir) $(builttest)

test: .PHONY
	./runtests.sh

.PHONY:
