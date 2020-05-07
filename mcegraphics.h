/**************************************************************************************
* Filename:   mcegraphics.h
* Author:     Rafel Amer (rafel.amer AT upc.edu)
* Copyright:  Rafel Amer 2018
* Disclaimer: This code is presented "as is" and it has been written to 
*             implement basic graphics compression based on Haar Wavelets 
*             and two dimensional cosine transform. It has been developed
*             for educational purposes. 
*	    
* License:    This library  is free software; you can redistribute it and/or
*             modify it under the terms of either:
*
*             1 the GNU Lesser General Public License as published by the Free
*               Software Foundation; either version 3 of the License, or (at your
*               option) any later version.
*
*             or
*
*             2 the GNU General Public License as published by the Free Software
*               Foundation; either version 2 of the License, or (at your option)
*               any later version.
*
*	      See https://www.gnu.org/licenses/
***************************************************************************************/
#ifndef H_MCE_GRAPHICS_H
#define H_MCE_GRAPHICS_H 1

#include <pam.h>
#include <zlib.h>
/*
   Images

   The image_data struct is based on chapter 14 of the book

   Programming Projects in C for Students of
   Engineering, Science and Mathematics
   Rouben Rostamian
   SIAM
   Computational Science & Engineering (2014)
   ISBN 978-1-611973-49-5
   https://userpages.umbc.edu/~rostamia/cbook/
 */
typedef struct {
  struct pam pam;
  double **r;
  double **g;
  double **b;
} image_data;
typedef image_data *Image;

#define freeImage(n) free_image(&(n))

Image read_image(char *filename);
void write_image(char *filename,Image img);
void free_image(Image *img);

/*
  Params
 */
typedef struct 
{
  double quality;
  double *vector;
  short *data;
  unsigned char *cdata;
  unsigned short **zigzag;
  double **T;
  unsigned char transform;
  unsigned char compress;
  unsigned int depth;
  unsigned int size;
  unsigned int height;
  unsigned int width;	
  FILE *inout;
} function_params;
typedef function_params *Params;
/*
  Haar Wavelets Transform
 */
#define WT_FWD 0
#define WT_REV 1

void haar_transform_vector(double *v, int n, int dir);
void haar_transform_matrix(double **a, int m, int n, int dir);

/*
  Discrete cosine transform
 */
#define DCT_FWD 0
#define DCT_REV 1
#define DCT_UNSCALED 0
#define DCT_SCALED 1

void dct_vector(double *v, int n, int dir,int scaled);
void dct_matrix(double **a, int m, int n, int dir,int scaled);

/*
  Compress and uncompress data
 */
#define TRANSFORMHAAR 0
#define TRANSFORMDCT 1
#define COMPRESSZLIB 0
#define COMPRESSWSQ 1

void zigzag_matrix(unsigned short **m,int size);
void zlib_compress_data(short *in,unsigned char *out,int insize,unsigned short *l);
void zlib_uncompress_data(short *out,char *in,int size,FILE *stream);
void wsq_compress_data(short *in,unsigned char *out,int insize,int *outsize);
void wsq_uncompress_data(short *out,int size,FILE *stream);

/*
  File read and write
 */
void file_write(void *data,size_t size,size_t nmemb,FILE *stream);
void file_read(void *data,size_t size,size_t nmemb,FILE *stream);
void compress_and_save(double **mat,short *vec,unsigned char *v,unsigned short **zigzag,int size,int method,FILE *stream);
void read_and_uncompress(double **mat,short *vec,unsigned char *v,unsigned short **zigzag,int size,int method,FILE *stream);

/*
  Matrix split and prune
 */
#define RESTOREMATRIX 1
#define COPYMATRIX 2
#define TRANSFORMMATRIX 3

typedef void (*CompressGrayScale)(double **,void *);
typedef void (*CompressColor)(double **,double **,double **,void *);
typedef void (*ProcessSubMatrix)(double **,double **,double **,int,void *);

void part_of_matrix(double **g,double **s,int rows,int columns,int size,int i, int j);
void restore_part_of_matrix(double **g,double **s,int rows,int columns,int size,int i, int j);
void foreach_submatrix(double **g,int rows,int columns,int size,unsigned char restore,CompressGrayScale f,void *params);
void foreach_submatrices(double **red,double **green,double **blue,int rows,int columns,int size,unsigned char restore,CompressColor f,void *params);
void prune_matrix_by_percentage(double **g,double *vec,int m,int n,int quality);
void prune_matrices_by_percentage(double **r,double **g,double **b,double **T,double *vec,int m,int n,int quality);

/*
  Compress and uncompress submatrices
 */
void compress_submatrix(double **r,double **g,double **b,int size,void *params);
void uncompress_submatrix(double **r,double **g,double **b,int size,void *params);

/*
  Color space
 */
void RGBToYCC(double *s, double *t,double *z);
void YCCToRGB(double *s, double *t,double *z);

#endif /* H_MCE_GRAPHICS_H */
