#include <mcegraphics.h>
#include <math.h>
#include <stdlib.h>
#include <uncmdline.h>

#define TRANSFORMHAAR 0
#define TRANSFORMDCT 1
#define COMPRESSZLIB 0
#define COMPRESSWSQ 1

typedef struct 
{
	int height;
	int width;
	int depth;
	short *data;
	unsigned char *cdata;
	unsigned short **zigzag;
	unsigned short size;
	unsigned char transform;
	unsigned char compress;
	FILE *input;
} Params;

