#include <mcegraphics.h>
#include <math.h>
#include <stdlib.h>
#include <cmdline.h>

#define TRANSFORMHAAR 0
#define TRANSFORMDCT 1

typedef struct 
{
	double quality;
	double *vector;
	short *data;
	unsigned char *cdata;
	unsigned short **zigzag;
	double **T;
	unsigned short size;
	unsigned char transform;
	unsigned char compress;
	FILE *output;
} Params;

