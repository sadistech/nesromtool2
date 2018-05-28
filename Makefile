srcdir=./src
libsrcdir=$(srcdir)/nesromtool

builtdir=./bin
builtfile=$(builtdir)/nesromtool2

files=$(libsrcdir)/header.c $(libsrcdir)/prg.c $(libsrcdir)/chr.c $(srcdir)/main.c 

all: $(builtfile)

$(builtdir):
	mkdir $(builtdir)

$(builtfile): ./bin $(files)
	gcc -o $(builtfile) $(files)

run: all
	$(builtfile) verify ./mario3.nes

clean:
	rm -rf $(builtdir)
