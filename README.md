# nesromtool2

Welcome to version 2 of `nesromtool`; a complete rewrite and rethinking of the original
[`nesromtool`](https://github.com/sadistech/nesromtool). The goal of the rewrite is to apply everything that
I've since learned about software engineering in general as well as my dust of my old C skills and package up
something interesting. One of my bigger regrets has been never truly finishing the original `nesromtool` and
at this point, this tool has covered all of the bases (and more) of what I had originally intended when I
started working on it in 2003.

## What is it?

`nrt` (as it shall now be referred) is a commandline tool used to aide work with NES ROM files in the iNES v1
format. Currently, where it really shines is in working with title metadata and hacking up the graphics data
in games. It has methods to import/export tile data either as raw CHR banks or to/from PNG and also provides
facilities for extraction of programming data and reporting on the overall structure of a ROM file.

This readme will illustrate some use-cases and help get you started.

### What's the state of this project?

As of this writing, this project should be considered alpha-level software. For the most part, the parts that
work work well, but the featureset is far from complete and error handling/detection isn't the best. For
example, it doesn't gracefully report missing or invalid arguments. However, the help system is implemented,
so we have that going for us, which is nice.

This has also mostly only been tested in macOS. I just haven't had a chance to build/test it on other OSs yet
since I've been head-down trying to get a baseline codebase working, but I *should* work, I would expect.

## Getting started

### Requirements

`nrt` requires standard build tools like `gcc` and `make` and also relies on `libpng`. It's been tested on
`1.6.34` and works great. I did attempt to build it on an older system that had a slightly older version of
`libpng` and wasn't able to compile it due to API changes, so make sure you've got something recent.

### Building and installing

The first thing you should do to get started is to clone this repo and `cd` into it. Then run the tests:

    $ make test

Once that's complete, you should be able to compile the project without issue:

    $ make

That should leave you with a file `./bin/nrt` that you can copy somewhere into your `PATH` and start using:

    $ cp bin/nrt /usr/local/bin

> In the future, I may swith to autotools for building so that build issues will be less of a thing and there
> will also be something like a `make install` process. But that time is not now.

### Usage

`nrt` follows a similar commandline usage pattern throughout:

    nrt <subcommand> [ <subcommand-args> ... ]

And most subcommands follow this pattern:

    nrt <subcommand> <action> <rom> [ <options> ... ] [ <action-args> ]

You can get help with the `--help` flag, either as a direct argument to `nrt` or as an argument to any
subcommand or subcommand's action.

The remainder of this section will talk about some basic, specific usage.

#### Getting information about a ROM

In order to get a dump of information about a ROM file, use the `info` subcommand:

```
$ nrt info mario.nes
Path: mario.nes
Valid: OK
PRG Banks: 2
CHR Banks: 1

Mapper lower nybble: 1
Ignore mirroring control: 0
Trainer present: 0
Persistent memory: 0
Mirror mode: 0

Mapper upper nybble: 0
Flag format: 0
Playchoice: 0
VS Unisystem: 0

PRG RAM Size: 0

TV System: 0

Bus conflicts: 0
PRG RAM absent: 0
TV System (2): 0

Title:
```

The above output shows that the ROM is "valid" (meaning that it's got a valid header and the file is the
correct size based on the fields in the header). It also shows some raw data about the state of the header. In
the future, the format of this output will be improved.

#### Extracting a PNG of graphics

`nrt` also provides a method of extracting a PNG file from a ROM. This is done via the `chr` subcommand (most NES
ROMs use special "CHR" banks to store graphics in the form of 256 tile blocks of 8x8 sprites).

To extract a bank of tiles, use the `extract` action of the `chr` subcommand:

    nrt chr extract <rom> [ --width=<width> ] <bank-index> <outfile>

For example:

    $ nrt chr extract mario.nes 0 mario-bank0.png

The above example will extract bank 0 (the first bank) from the file `mario.nes` and write it to
`mario-bank0.png` as a png bitmap file. `nrt` decides to write as a png
based on the file extension of the `<outfile>`.

> Extracted pngs are 2-bit indexed color since the NES can only handle sprites with 4-color palettes, so when
> replacing a bank with a png file, the png file MUST be indexed color and will use the first 4 (0-3)
> colors in the palette only.

#### Replacing the bank with a modified PNG

Once you've modified the png in your graphics editor of choice, you can replace the entire CHR bank with the
updated file. Just make sure that the file is still indexed color and you haven't use any but the original 4
colors in the color palette.

When replacing a bank of tiles, the command usage is:

    nrt chr replace <rom> [ --outfile=<outfile> ] <bank-index> <source-file>

For example:

    $ nrt chr replace mario.nes --outfile "mario-edit.nes" 0 mario-bank0-edit.png

By default, `nrt` will overwrite the original ROM file, but by using the `--outfile` argument, we can tell it
to write to a different location. This is recommended since you never know when you are gonna corrupt your
ROM.

### License

This repo is licensed with the MIT license. I hope it's useful to you.

### Credits, etc.

`nrt` is based on the original `nesromtool` written by Spike Grobstein (me), which utilized a multitude of
documents found on the 2003 internet, with the originals locations being lost to history. This rewrite relied
heavily on the original codebase as well as documentation found on
[wiki.nesdev.com](https://wiki.nesdev.com/).

