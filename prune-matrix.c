/**************************************************************************************
* Filename:   prune-matrix.c
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
#include <math.h>
#include <stdio.h>

static int cmpfunc(const void *a, const void *b) {
  if(*(double *)a > *(double *)b)
    return 1;
  if(*(double *)a < *(double *)b)
    return -1;
  return 0;
}

void prune_matrix_by_percentage(double **g,double *vec,int m,int n,int quality)
{
	int zeros = 0;
	matrix_to_vector(g,vec,m,n,double);
	for(int i = 0;i < m * n;i++)
		if(vec[i] < 0)
			vec[i] *= -1;
	qsort(vec,m*n,sizeof(double),cmpfunc);
	while((vec[zeros] == 0) && (zeros < m*n))
		zeros++;
	int k;
	double t;
	k = zeros+(100.0-quality)/100.0*(m*n - zeros);
	t = vec[k];
	for(int i=0;i<m;i++)
		for(int j=0;j<n;j++)
			if(fabs(g[i][j]) < t)
				g[i][j] = 0.0;
			else
				g[i][j] = lrint(g[i][j]);
}

void prune_matrices_by_percentage(double **r,double **g,double **b,double **T,double *vec,int m,int n,int quality)
{
	int zeros = 0;
	for(int i=0;i < m;i++)
		for(int j=0;j < n;j++)
		{
			double rv, gv, bv;
			rv = r[i][j];
			gv = g[i][j];
			bv = b[i][j];
			T[i][j] = sqrt(rv*rv+gv*gv+bv*bv);
		}
	matrix_to_vector(T,vec,m,n,double);
	qsort(vec,m * n,sizeof(double),cmpfunc);
	while((vec[zeros] == 0) && (zeros < m * n))
		zeros++;
	int k;
	double t;
	k = zeros+(100.0-quality)/100.0*(m * n - zeros);
	t = vec[k];
	k = 0;
	for(int i=0;i < m;i++)
		for(int j=0;j < n;j++)
		{
			if(T[i][j] < t)
			{
				r[i][j] = 0.0;
				g[i][j] = 0.0;
				b[i][j] = 0.0;
				k++;
			}
			else
			{
				r[i][j] = lrint(r[i][j]);
				g[i][j] = lrint(g[i][j]);
				b[i][j] = lrint(b[i][j]);
			}
		}
}
