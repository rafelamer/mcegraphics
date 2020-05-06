/**************************************************************************************
* Filename:   color-space.c
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

void RGBToYCC(double *s, double *t,double *z)
{
  double r, g, b;
  r = *s;
  g = *t;
  b = *z;
  *s = 16.0 + 65.481 * r + 128.553 * g + 24.966 * b;
  *t = 128.0 - 37.797 * r - 74.203 * g + 112.0 * b;
  *z = 128.0 + 112.0 * r - 93.786 * g - 18.214 * b;
}

void YCCToRGB(double *s, double *t,double *z)
{
  double y, cb, cr;
  y = *s;
  cb = *t;
  cr = *z;
  *s = -0.874202 + 0.00456621 * y + 0.00625893 * cr;
  *t = 0.531668 - 0.00153632 * cb - 0.00318811 * cr + 0.00456621 * y;
  *z = 1.08563 + 0.00791072 * cb + 0.00456621 * y;
}

