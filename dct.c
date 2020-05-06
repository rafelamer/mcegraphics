/**************************************************************************************
* Filename:   dct.c
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
#include <math.h>
#include <fftw3.h>
#include <string.h>
#include <stdlib.h>

static void dct_vector_forward(double *v, int n,int scaled)
{
  double *inout;
  fftw_plan plan;
  
  inout = (double *)fftw_malloc(n*sizeof(double));
  memcpy(inout,v,n*sizeof(double));
  plan = fftw_plan_r2r_1d(n,inout,inout,FFTW_REDFT10,FFTW_ESTIMATE);
  fftw_execute(plan);
  /*
    Scaling factor such that dct_vector_forward is equal to matlab dct
   */
  if(scaled == DCT_SCALED)
  {
    double f = sqrt(1.0/(2*n));
    for(int i = 0; i < n; i++)
      inout[i] *= f;
    inout[0] *= 1.0/sqrt(2.0);
  }
  else if(scaled != DCT_UNSCALED)
    printf("Scaled must be DCT_SCALED or DCT_SCALED");

  memcpy(v,inout,n*sizeof(double));
  fftw_destroy_plan(plan);
  fftw_free(inout);
}

static void dct_vector_reverse(double *v, int n,int scaled)
{
  double *inout;
  fftw_plan plan;
  /*
    Scaling factor such that dct_vector_reverse is equal to matlab idct
   */
  if(scaled == DCT_SCALED)
  {
    double f = 2*n*sqrt(1.0/(2*n));
    for(int i = 0; i < n; i++)
      v[i] /= f;
    v[0] /= 1.0/sqrt(2.0);
  }
  else if(scaled != DCT_UNSCALED)
    printf("Scaled must be DCT_SCALED or DCT_SCALED");
  
  inout = (double *)fftw_malloc(n*sizeof(double));
  memcpy(inout,v,n*sizeof(double));
  plan = fftw_plan_r2r_1d(n,inout,inout,FFTW_REDFT01,FFTW_ESTIMATE);
  fftw_execute(plan);
  memcpy(v,inout,n*sizeof(double));
  fftw_destroy_plan(plan);
  fftw_free(inout);
}

static void dct_matrix_forward(double **a,int m,int n,int scaled)
{
  double *inout, *aux;
  fftw_plan plan;
  
  inout = (double *)fftw_malloc(m*n*sizeof(double));
  aux = inout;
  for(int i=0;i<m;i++)
  {
    memcpy(aux,a[i],n*sizeof(double));
    aux += n;
  }
  plan = fftw_plan_r2r_2d(m,n,inout,inout,FFTW_REDFT10,FFTW_REDFT10,FFTW_ESTIMATE);
  fftw_execute(plan);

  aux = inout;
  for(int i=0;i<m;i++)
  {
    memcpy(a[i],aux,n*sizeof(double));
    aux += n;
  }
  /*
    Scaling factor such that dct_matrix_forward is equal to matlab dct2
   */
  if(scaled == DCT_SCALED)
  {
    double f = 1.0/(4.0*sqrt(m/2.0)*sqrt(n/2.0));
    for(int i = 0; i < m; i++)
      for(int j = 0; j < n; j++)  
	a[i][j] *= f;
    for(int i = 0; i < m; i++)
      a[i][0] *= 1.0/sqrt(2);
    for(int j = 0; j < n; j++)
      a[0][j] *= 1.0/sqrt(2);
  }
  else if(scaled != DCT_UNSCALED)
    printf("Scaled must be DCT_SCALED or DCT_SCALED");

  fftw_destroy_plan(plan);
  fftw_free(inout);
}

static void dct_matrix_reverse(double **v,int m,int n,int scaled)
{
  double *inout, *aux;
  fftw_plan plan;

  /*
    Scaling factor such that dct_matrix_forward is equal to matlab dct2
   */
  if(scaled == DCT_SCALED)
  {
    double f = sqrt(m/2.0)*sqrt(n/2.0)/(m*n);
    for(int i = 0; i < m; i++)
      for(int j = 0; j < n; j++)  
	v[i][j] *= f;
    for(int i = 0; i < m; i++)
      v[i][0] *= sqrt(2);
    for(int j = 0; j < n; j++)
      v[0][j] *= sqrt(2);
  }
  else if(scaled != DCT_UNSCALED)
    printf("Scaled must be DCT_SCALED or DCT_SCALED");
  inout = (double *)fftw_malloc(m*n*sizeof(double));
  aux = inout;
  for(int i=0;i<m;i++)
  {
    memcpy(aux,v[i],n*sizeof(double));
    aux += n;
  }
  plan = fftw_plan_r2r_2d(m,n,inout,inout,FFTW_REDFT01,FFTW_REDFT01,FFTW_ESTIMATE);
  fftw_execute(plan);
  aux = inout;
  for(int i=0;i<m;i++)
  {
    memcpy(v[i],aux,n*sizeof(double));
    aux += n;
  }
  fftw_destroy_plan(plan);
  fftw_free(inout); 
}

void dct_vector(double *v, int n, int dir,int scaled)
{
  if (dir == DCT_FWD)
    dct_vector_forward(v, n,scaled);
  else if (dir == DCT_REV)
    dct_vector_reverse(v, n,scaled);
  else
  { // shouldn’t be here!
    fprintf(stderr, "*** error in dct_vector(): "
	    "the third argument should be one of "
	    "DCT_FWD or DCT_REV\n");
    exit(EXIT_FAILURE);
  }
}
  
void dct_matrix(double **a, int m, int n, int dir,int scaled)
{
  if (dir == DCT_FWD)
    dct_matrix_forward(a,m,n,scaled);
  else if (dir == DCT_REV)
    dct_matrix_reverse(a,m,n,scaled);
  else
  { // shouldn’t be here!
    fprintf(stderr, "*** error in dct_matrix(): "
	    "the third argument should be one of "
	    "DCT_FWD or DCT_REV\n");
    exit(EXIT_FAILURE);
  }
}

