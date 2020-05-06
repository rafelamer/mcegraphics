CFLAGS = -O3 -I.
LIBS = -lmcegrahics -lnetpbm -lfftw3 -lm -lz
CC = gcc
TARGET1 = mcecompress 
TARGET2 = mceuncompress

$(TARGET1): mcecompress.o cmdline.o
	$(CC) -o $(TARGET1) mcecompress.o cmdline.o $(LIBS)

$(TARGET2): mceuncompress.o uncmdline.o
	$(CC) -o $(TARGET2) mceuncompress.o uncmdline.o $(LIBS)

cmdline.c: cmdline.ggo
	gengetopt --input=cmdline.ggo

uncmdline.c: uncmdline.ggo
	gengetopt --input=uncmdline.ggo --file-name=uncmdline --func-name=uncmdline_parser

cmdline.o: cmdline.c
	$(CC) -c -o cmdline.o $(CFLAGS) cmdline.c

uncmdline.o: uncmdline.c
	$(CC) -c -o uncmdline.o $(CFLAGS) uncmdline.c

all: cmdline.o uncmdline.o $(TARGET1) $(TARGET2)

install:
	cp $(TARGET1) $(TARGET2) /usr/local/bin/

clean:
	rm -f $(OBJECTS) cmdline.o cmdline.c cmdline.h uncmdline.o uncmdline.c uncmdline.h mcecompress.o mceuncompress.o $(TARGET1) $(TARGET2)
