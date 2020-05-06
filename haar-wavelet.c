/**************************************************************************************
* Filename:   haar-wavelet.c
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
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <array.h>

#define SQRT1_2 sqrt(1.0/2)

static void haar_transform_vector_forward(double *v, int n)
{
  int k;
  double *y;

  make_vector(y,n);
  memset(y,0,n*sizeof(double));
    
  k = 1;
  while (2*k <= n)
    k *= 2;
  while (k > 1)
  {
    k = k / 2;
    for (int i=0;i<k;i++)
    {
      y[i]   = SQRT1_2*(v[2*i] + v[2*i+1]);
      y[i+k] = SQRT1_2*(v[2*i] - v[2*i+1]);
    }
    memcpy(v,y,2*k*sizeof(double));
  }
  free_vector(y);
}

static void haar_transform_vector_reverse(double *v, int n)
{
  int k;
  double *y;

  make_vector(y,n);
  memset(y,0,n*sizeof(double));

  k = 1;
  while (2*k <= n )
  {
    for (int i=0;i<k;i++)
    {
      y[2*i]   = SQRT1_2*(v[i] + v[i+k]);
      y[2*i+1] = SQRT1_2*(v[i] - v[i+k]);
    }
    memcpy(v,y,2*k*sizeof(double));
    k *= 2;
  }
  free_vector(y);
}

static void haar_transform_matrix_forward(double **a, int m, int n)
{
  int k;
  double **y;

  make_matrix(y,m,n);
  for(int i=0;i<m;i++)
     memcpy(y[i],a[i],m*sizeof(double));
  /*
    Haar transform of each column
   */
  k = 1;
  while (2*k <= m )
    k *= 2;
  while(k > 1)
  {
    k = k / 2;
    for (int j=0;j<n;j++)
    {
      for (int i=0;i<k;i++)
      {
        y[i][j] = SQRT1_2*(a[2*i][j] + a[2*i+1][j]);
        y[k+i][j] = SQRT1_2*(a[2*i][j] - a[2*i+1][j]);
      }
    }
    for(int i=0;i<2*k;i++)
      memcpy(a[i],y[i],n*sizeof(double));
  }
  /*
    Haar transform of each row
   */
  k = 1;
  while (2*k <= n)
    k *= 2;
  while (k > 1)
  { 
    k = k / 2;
    for (int j=0;j<k;j++)
    {
      for (int i=0;i<m;i++)
      {
        y[i][j] = SQRT1_2*(a[i][2*j] + a[i][2*j+1]);
        y[i][k+j] = SQRT1_2*(a[i][2*j] - a[i][2*j+1]);
      }
    }
    for (int i = 0;i < m;i++)
       memcpy(a[i],y[i],2*k*sizeof(double));
  }
 free_matrix(y);
}

static void haar_transform_matrix_reverse(double **a, int m, int n)
{
  int k;
  double **y;

  
  make_matrix(y,m,n);
  for(int i=0;i<m;i++)
    memcpy(y[i],a[i],m*sizeof(double));
  /*
    Inverse Haar transform of each row
   */
  k = 1;
  while (2*k <= n)
  {
    for(int j=0;j<k;j++)
      for(int i=0;i<m;i++)
      {
	y[i][2*j] = SQRT1_2*(a[i][j] + a[i][j+k]);
	y[i][2*j+1] = SQRT1_2*(a[i][j] - a[i][j+k]);
      }
    for (int i=0;i<m;i++)
      memcpy(a[i],y[i],2*k*sizeof(double));
    k *= 2;
  }
  /*
    Inverse Haar transform of each column
   */
  k = 1;

  while(2*k <= m )
  {
    for (int j=0;j<n;j++)
      for (int i=0;i<k;i++)
      {
        y[2*i][j] =  SQRT1_2*(a[i][j] + a[i+k][j]);
        y[2*i+1][j] = SQRT1_2*(a[i][j] - a[i+k][j]);
      }
    for (int i=0;i<2*k;i++)
      memcpy(a[i],y[i],n*sizeof(double));
    k *= 2;
  }
  free_matrix (y);
}

void haar_transform_vector(double *v, int n, int dir)
{
  if (dir == WT_FWD)
    haar_transform_vector_forward(v, n);
  else if (dir == WT_REV)
    haar_transform_vector_reverse(v, n);
  else
  { // shouldn’t be here!
    fprintf(stderr, "*** error in haar_transform_vector(): "
	    "the third argument should be one of "
	    "WT_FWD or WT_REV\n");
    exit(EXIT_FAILURE);
  }
}

void haar_transform_matrix(double **a, int m, int n, int dir)
{
  if (dir == WT_FWD)
    haar_transform_matrix_forward(a,m,n);
  else if (dir == WT_REV)
    haar_transform_matrix_reverse(a,m,n);
  else
  { // shouldn’t be here!
    fprintf(stderr, "*** error in haar_transform_matrix(): "
	    "the third argument should be one of "
	    "WT_FWD or WT_REV\n");
    exit(EXIT_FAILURE);
  }
}
