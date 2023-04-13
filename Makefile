CFLAGS = -fPIC -O3 -I. -I/usr/include/netpbm
CC = gcc
TARGET = libmcegraphics.so.1.0.0
OBJECTS = xmalloc.o image-io.o haar-wavelet.o dct.o data-compress.o file-rw.o \
          submatrix-compress.o prune-matrix.o split-matrix.o color-space.o

INCLUDES = mcegraphics.h array.h

$(TARGET): $(OBJECTS)
	$(CC) -shared -fPIC -Wl,-soname,libmcegraphics.so.1 -o $(TARGET) $(OBJECTS)

install: $(TARGET)
	cp $(INCLUDES) /usr/local/include/
	cp $(TARGET) /usr/local/lib/
	ln -sf       /usr/local/lib/$(TARGET) /usr/local/lib/libmcegraphics.so.1
	ln -sf       /usr/local/lib/$(TARGET) /usr/local/lib/libmcegraphics.so

clean:
	rm -f $(OBJECTS) $(TARGET)
