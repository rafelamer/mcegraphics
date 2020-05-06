/**************************************************************************************
* Filename:   submatrix-compress.c
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

void compress_submatrix(double **r,double **g,double **b,int size,void *params)
{
  Params p = (Params)params;
  if (p->transform == TRANSFORMDCT)
    {
      dct_matrix(r,size,size,DCT_FWD,DCT_SCALED);
      if (p->depth > 1)
	{
	  dct_matrix(g,size,size,DCT_FWD,DCT_SCALED);
	  dct_matrix(b,size,size,DCT_FWD,DCT_SCALED);
	}
    }
  else
    {
      haar_transform_matrix(r,size,size,WT_FWD);
      if (p->depth > 1)
	{
	  haar_transform_matrix(g,size,size,WT_FWD);
	  haar_transform_matrix(b,size,size,WT_FWD);
	}
    }
  if (p->depth == 1)
    {
      prune_matrix_by_percentage(g,p->vector,size,size,p->quality);
      compress_and_save(g,p->data,p->cdata,p->zigzag,size,p->compress,p->inout);
    }
  else
    {
      prune_matrices_by_percentage(r,g,b,p->T,p->vector,size,size,p->quality);
      compress_and_save(r,p->data,p->cdata,p->zigzag,size,p->compress,p->inout);
      compress_and_save(g,p->data,p->cdata,p->zigzag,size,p->compress,p->inout);
      compress_and_save(b,p->data,p->cdata,p->zigzag,size,p->compress,p->inout);
    }
}

void uncompress_submatrix(double **r,double **g,double **b,int size,void *params)
{
    Params p = (Params)params;
    read_and_uncompress(r,p->data,p->cdata,p->zigzag,size,p->compress,p->inout);
    if (p->depth > 1)
      {
	read_and_uncompress(g,p->data,p->cdata,p->zigzag,size,p->compress,p->inout);
	read_and_uncompress(b,p->data,p->cdata,p->zigzag,size,p->compress,p->inout);
      }
    if(p->transform == TRANSFORMDCT)
      {
	dct_matrix(r,size,size,DCT_REV,DCT_SCALED);
	if (p->depth > 1)
	  {
	    dct_matrix(g,size,size,DCT_REV,DCT_SCALED);
	    dct_matrix(b,size,size,DCT_REV,DCT_SCALED);
	  }
      }
    else
      {
	haar_transform_matrix(r,size,size,WT_REV);
	if (p->depth > 1)
	  {
	    haar_transform_matrix(g,size,size,WT_REV);
	    haar_transform_matrix(b,size,size,WT_REV);
	  }
      }
}
