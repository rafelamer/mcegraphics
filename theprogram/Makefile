CFLAGS = -O3 -I.
LIBS = -lmcegraphics -lnetpbm -lfftw3 -lm -lz
CC = gcc
TARGET1 = mcecompress 
TARGET2 = mceuncompress
OBJECTS = mcecompress.o mceuncompress.o cmdline.o uncmdline.o
INCLUDES = cmdline.h uncmdline.h

$(TARGET1): $(OBJECTS) $(INCLUDES)
	$(CC) -o $(TARGET1) mcecompress.o cmdline.o $(LIBS)

$(TARGET2): $(OBJECTS) $(INCLUDES)
	$(CC) -o $(TARGET2) mceuncompress.o uncmdline.o $(LIBS)

mcecompress.o: mcecompress.c cmdline.c
	$(CC) -c -o mcecompress.o $(CFLAGS) mcecompress.c

mceuncompress.o: mceuncompress.c uncmdline.c
	$(CC) -c -o mceuncompress.o $(CFLAGS) mceuncompress.c

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
