/**************************************************************************************
* Filename:   file-io.c
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
#include <mcegraphics.h>
#include <array.h>

void file_write(void *data,size_t size,size_t nmemb,FILE *stream)
/*
  Write nmemb members of size size of data to file
 */
{
	if(fwrite(data,size,nmemb,stream) != nmemb)
  	{
    	fprintf(stderr,"Error writing a file\n");
    	exit(EXIT_FAILURE);
  	}
}

void file_read(void *data,size_t size,size_t nmemb,FILE *stream)
/*
  Read nmemb members of size size of data from file
 */
{
	if(fread(data,size,nmemb,stream) != nmemb)
  	{
    	fprintf(stderr,"Error reading a file\n");
    	exit(EXIT_FAILURE);
  	}
}

void compress_and_save(double **mat,short *vec,unsigned char *v,unsigned short **zigzag,int size,int method,FILE *stream)
/*
  Compres data in the matrix mat and writes iy to a file
  
  vec is a preallocated vector of length size * size
  v is a preallocated vector of length size * size
  zigzag is a matrix returned by the function zigzag_matrix
 */
{
  for(int i=0;i < size;i++)
    for(int j=0;j < size;j++)
      vec[zigzag[i][j]-1] = mat[i][j];

  if(method == COMPRESSZLIB)
  {
    unsigned short l;
    zlib_compress_data(vec,v,size*size,&l);
    file_write(&l,sizeof(unsigned short),1,stream);
    file_write(v,l,1,stream);
  }
  else
  {
    int l;
    wsq_compress_data(vec,v,size*size,&l);
    file_write(v,l,1,stream);
  }

}

void read_and_uncompress(double **mat,short *vec,unsigned char *v,unsigned short **zigzag,int size,int method,FILE *stream)
/*
  Reads data from a file, uncompress it and store it to the mnatrix mat 
  
  mat is a preallocated matrix of simension size * size
  vec is a preallocated vector of length size * size
  v is a preallocated vector of length size * size
  zigzag is a matrix returned by the function zigzag_matrix
 */
{
  if(method == COMPRESSZLIB)
    zlib_uncompress_data(vec,v,size*size,stream); 
  else
    wsq_uncompress_data(vec,size*size,stream);

  for(int i=0;i < size;i++)
    for(int j=0;j < size;j++)
      mat[i][j] = vec[zigzag[i][j]-1];
    
}