srcdir=./src
libsrcdir=$(srcdir)/nrt

builtdir=./bin
builtfile=$(builtdir)/nrt

srcfiles=$(libsrcdir)/error.c $(libsrcdir)/util.c $(libsrcdir)/header.c $(libsrcdir)/prg.c $(libsrcdir)/chr.c $(srcdir)/main.c 
headers=$(libsrcdir)/error.h $(libsrcdir)/util.h $(libsrcdir)/header.h $(libsrcdir)/prg.h $(libsrcdir)/chr.h $(srcdir)/nrt.h

all: $(builtfile)

$(builtdir):
	mkdir $(builtdir)

$(builtfile): ./bin $(srcfiles) $(headers)
	gcc -o $(builtfile) $(srcfiles)

run: $(builtfile) all
	$(builtfile) verify ./mario3.nes

clean:
	rm -rf $(builtdir)
