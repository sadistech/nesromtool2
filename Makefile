srcdir=./src
libsrcdir=$(srcdir)/nrt

builtdir=./bin
builtfile=$(builtdir)/nrt

srcfiles=$(libsrcdir)/error.c $(libsrcdir)/util.c $(libsrcdir)/prg.c $(libsrcdir)/chr.c $(libsrcdir)/header.c $(srcdir)/main.c 
headers=$(libsrcdir)/error.h $(libsrcdir)/util.h $(libsrcdir)/prg.h $(libsrcdir)/chr.h $(libsrcdir)/header.h $(srcdir)/nrt.h

all: $(builtfile)

$(builtdir):
	mkdir $(builtdir)

$(builtfile): ./bin $(srcfiles) $(headers)
	gcc -o $(builtfile) $(srcfiles)

run: $(builtfile) all
	$(builtfile) verify ./mario3.nes

clean:
	rm -rf $(builtdir)
