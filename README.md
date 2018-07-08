# nesromtool

 * verify
 * extract prg
 * extract chr
 * extract tile
 * extract title
 * set title
 * info
 * replace PRG
 * replace CHR
 * insert PRG
 * insert CHR

# advanced

 * patch
 * convert tile
 * replace tile
 * convert tile
 * create sprite
 * convert sprite (to tiles)
 * decompile
 * compile

## usage:

### ROMs

 * validate
 * print info

    nrt validate file.nes
    nrt info file.nes

### Titles

 * print
 * set

    nrt title set file.nes "This NES ROM"
    nrt title get file.nes

### CHRs

 * extract an entire CHR bank to a file
 * extract a CHR bank to a PNG
 * import a png as a CHR bank
 * import a raw CHR and replace an existing bank
 * delete a CHR
 * insert a CHR

    nrt chr extract file.nes 0 bank0.chr
    nrt chr extract file.nes 0 bank0.png
    nrt chr extract file.nes --format=png 0 bank0
    nrt chr extract file.nes --format=chr 1 bank1
    nrt chr replace file.nes 0 bank0.png
    nrt chr insert file.nes 0 bank0.png
    nrt chr delete file.nes 0


