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
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <array.h>

void part_of_matrix(double **g,double **s,int rows,int columns,int size,int i, int j)
{
	int c, r;

	r = rows/size;
	if(size*r== rows)
		r--;
	c = columns/size;
	if(size*c == columns)
		c--;
	if((i > r) || (j > c))
	{
		fprintf(stderr,"Error splitting the matrix. Invalid indices: %d and %d\n",i,j);
      	exit(EXIT_FAILURE);
	}
	for(int m=0;m < size;m++)
		for(int n=0;n < size;n++)
		{
			if((size * i + m < rows) && (size * j + n < columns))
			{
				s[m][n] = g[size * i + m][size * j + n] - 128;
			}
			else if((size * i + m >= rows) && (size * j + n >= columns))
			{
				int k1 = size * i + m;
				int k2 = size * j + n;
				k1 = k1 - k1 % rows - 1;
				k2 = k2 - k2 % columns - 1;
				s[m][n] = g[k1][k2] - 128;
			}
			else if(size * i + m >= rows)
			{
				int k1 = size * i + m;
				k1 = k1 - k1 % rows - 1;
				s[m][n] = g[k1][size * j + n] - 128;
			}
			else if(size * j + n >= columns)
			{
				int k2 = size * j + n;
				k2 = k2 - k2 % columns - 1;
				s[m][n] = g[size * i + m][k2] - 128;
			}
		}
}

void restore_part_of_matrix(double **g,double **s,int rows,int columns,int size,int i, int j)
{
	int c, r;
	r = rows/size;
	if(size * r== rows)
		r--;
	c = columns/size;
	if(size * c == columns)
		c--;
	if((i > r) || (j > c))
	{
		fprintf(stderr,"Error restoring the matrix. Invalid indices: %d and %d\n",i,j);
      	exit(EXIT_FAILURE);
	}
	for(int m=0;m < size;m++)
		for(int n=0;n < size;n++)
			if((size * i + m < rows) && (size * j + n < columns))
				g[size * i + m][size * j + n] = lrint(s[m][n]) + 128;
}

/*
The use of the function foreach_submatrices are possible in three ways,
  depending of the value of the parameter restore

  1. If restore == SAVEMATRIX
     we apply the function f to a copy of every submatrix of
     red, green and blue. Note that red, green and blue are not modified
     during the process. The we have to save the values. If we don't do

  2. If restore == RESTOREMATRIX
     We apply the function f to fill the matrices rs, gs, bs and then copy
     the contents of that matrix to red, green and blue.

  3. If restore == TRANSFORMMATRIX
     We copy the submatrices, apply the functions f and then restore the
     values of the modified matrices

  In the main program we will define the function f and use the function 
  foreach_submatrices. The problem is how to pass some extra data
  or parameters to the function f. That's the purpose of the last parameter 
  of the function foreach_submatrices

  void *param
 */

void foreach_submatrix(double **g,int rows,int columns,CompressGrayScale f,void *params)
{
	int c, r;
	double **s;
	Params p = (Params)params;
	int size = p->size;

	make_matrix(s,p->size,p->size);
	r = rows/size;
	if(size * r== rows)
		r--;
	c = columns/size;
	if(size * c == columns)
		c--;
	for(int i=0;i<=r;i++)
		for(int j=0;j<=c;j++)
		{
				if (p->action == SAVEMATRIX)
				{
					part_of_matrix(g,s,rows,columns,size,i,j);
					f(s,params);
				}
				else if (p->action == RESTOREMATRIX)
				{
					f(s,params);
					restore_part_of_matrix(g,s,rows,columns,size,i,j);
				}
				else if (p->action == OVERWRITEMATRIX)
				{
					part_of_matrix(g,s,rows,columns,size,i,j);
					f(s,params);
					restore_part_of_matrix(g,s,rows,columns,size,i,j);
				}
		}
	free_matrix(s);
}

void foreach_submatrices(double **red,double **green,double **blue,int rows,int columns,CompressColor f,void *params)
{
	int c, r;
	double **rs, **gs, **bs;
	Params p = (Params)params;
	int size = p->size;
	
	make_matrix(rs,size,size);
	make_matrix(gs,size,size);
	make_matrix(bs,size,size);
	r = rows/size;
	if(size * r == rows)
		r--;
	c = columns/size;
	if(size * c == columns)
		c--;
	
	for(int i=0;i<=r;i++)
	{
		for(int j=0;j<=c;j++)
		{
			if (p->action == SAVEMATRIX)
			{
				part_of_matrix(red,rs,rows,columns,size,i,j);
				part_of_matrix(green,gs,rows,columns,size,i,j);
				part_of_matrix(blue,bs,rows,columns,size,i,j);
				f(rs,gs,bs,params);
			}
			else if (p->action == RESTOREMATRIX)
			{
				f(rs,gs,bs,params);
				restore_part_of_matrix(red,rs,rows,columns,size,i,j);
				restore_part_of_matrix(green,gs,rows,columns,size,i,j);
				restore_part_of_matrix(blue,bs,rows,columns,size,i,j);
			}
			else if (p->action == OVERWRITEMATRIX)
			{
				part_of_matrix(red,rs,rows,columns,size,i,j);
				part_of_matrix(green,gs,rows,columns,size,i,j);
				part_of_matrix(blue,bs,rows,columns,size,i,j);
				f(rs,gs,bs,params);
				restore_part_of_matrix(red,rs,rows,columns,size,i,j);
				restore_part_of_matrix(green,gs,rows,columns,size,i,j);
				restore_part_of_matrix(blue,bs,rows,columns,size,i,j);
			}
		}
	}
	free_matrix(rs);
	free_matrix(gs);
	free_matrix(bs);
}

