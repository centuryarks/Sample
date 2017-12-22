CC = gcc

LIBTIFF_INC = -I./local/include
LIBTIFF_LIB = local/lib/libtiff.a

all: raw2dng raw2hdr

tiff-3.8.2.tar.gz:
	wget -nc 'http://dl.maptools.org/dl/libtiff/tiff-3.8.2.tar.gz'

tiff-3.8.2: tiff-3.8.2.tar.gz libtiff.patch
	tar -zxvf "$<"
	cat "libtiff.patch" | patch -p0 -f

libtiff.patch:
	wget -nc 'http://www.cybercom.net/~dcoffin/dcraw/libtiff.patch'

local/lib/libtiff.a: tiff-3.8.2
	cd $< ; ./configure --prefix=$(PWD)/local
	cd $< ; make -j4
	cd $< ; make install

raw2dng: raw2dng.c $(LIBTIFF_LIB)
	$(CC) -o raw2dng raw2dng.c $(LIBTIFF_INC) $(LIBTIFF_LIB) -ljpeg -lm -lz

raw2hdr: raw2hdr.c $(LIBTIFF_LIB)
	$(CC) -o raw2hdr raw2hdr.c $(LIBTIFF_INC) $(LIBTIFF_LIB) -ljpeg -lm -lz

clean:
	rm raw2dng raw2hdr
