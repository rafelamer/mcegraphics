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

cmdline.h: cmdline.ggo
	gengetopt --input=cmdline.ggo

uncmdline.h: uncmdline.ggo
	gengetopt --input=uncmdline.ggo --file-name=uncmdline --func-name=uncmdline_parser

mcecompress.o: mcecompress.c cmdline.c cmdline.h
	$(CC) -c -o mcecompress.o $(CFLAGS) mcecompress.c

mceuncompress.o: mceuncompress.c uncmdline.c uncmdline.h
	$(CC) -c -o mceuncompress.o $(CFLAGS) mceuncompress.c

cmdline.o: cmdline.c
	$(CC) -c -o cmdline.o $(CFLAGS) cmdline.c

uncmdline.o: uncmdline.c
	$(CC) -c -o uncmdline.o $(CFLAGS) uncmdline.c

all: $(TARGET1) $(TARGET2)

install:
	cp $(TARGET1) $(TARGET2) /usr/local/bin/

clean:
	rm -f $(OBJECTS) cmdline.o cmdline.c cmdline.h uncmdline.o uncmdline.c uncmdline.h mcecompress.o mceuncompress.o $(TARGET1) $(TARGET2)
