/**************************************************************************************
* Filename:   image-io.c
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
#include <array.h>

static void read_pgm_pixel_data(Image img)
{
  struct pam *pam = &img->pam;
  tuple *row = pnm_allocpamrow(pam);
  make_matrix(img->g, pam->height, pam->width);
  img->r = NULL;
  img->b = NULL;
  for (int i = 0; i < pam->height; i++) {
    pnm_readpamrow(pam, row);
    for (int j = 0; j < pam->width; j++)
      img->g[i][j] = row[j][0];
  }
  pnm_freepamrow(row);
}

static void read_ppm_pixel_data(Image img)
{
  struct pam *pam = &img->pam;
  tuple *row = pnm_allocpamrow(pam);
  make_matrix(img->r, pam->height, pam->width);
  make_matrix(img->g, pam->height, pam->width);
  make_matrix(img->b, pam->height, pam->width);
  for (int i = 0; i < pam->height; i++) {
    pnm_readpamrow(pam, row);
    for (int j = 0; j < pam->width; j++) {
      img->r[i][j] = row[j][0];      
      img->g[i][j] = row[j][1];
      img->b[i][j] = row[j][2];      
    }
  }
  pnm_freepamrow(row);
}

Image read_image(char *filename)
{
  Image img = xmalloc(sizeof(image_data));
  struct pam *pam = &(img->pam);
  FILE *fp = pm_openr(filename);
  pnm_readpaminit(fp, pam, sizeof(struct pam));
  if (pam->format == PGM_FORMAT || pam->format == RPGM_FORMAT)
    read_pgm_pixel_data(img);
  else if (pam->format == PPM_FORMAT || pam->format == RPPM_FORMAT)
    read_ppm_pixel_data(img);
  else {
    fprintf(stderr, "error: file ‘%s’ contains neither "
	    "a PGM nor a PPM image\n", filename);
    exit(EXIT_FAILURE);
  }
  pm_close(fp);
  return img;
}

static void write_pgm_pixel_data(Image img)
{
  struct pam *pam = &(img->pam);
  tuple *row = pnm_allocpamrow(pam);
  for (int i = 0; i < pam->height; i++) {
    for (int j = 0; j < pam->width; j++)
      row[j][0] = img->g[i][j];
    pnm_writepamrow(pam, row);
  }
  pnm_freepamrow(row);
}

static void write_ppm_pixel_data(Image img)
{
  struct pam *pam = &(img->pam);
  tuple *row = pnm_allocpamrow(pam);
  for (int i = 0; i < pam->height; i++) {
    for (int j = 0; j < pam->width; j++) {
      row[j][0] = img->r[i][j];
      row[j][1] = img->g[i][j];
      row[j][2] = img->b[i][j];      
    }
    pnm_writepamrow(pam, row);
  }
  pnm_freepamrow(row);
}

void write_image(char *filename, Image img)
{
  struct pam *pam = &(img->pam);
  pam->file = pm_openw(filename);
  pam->plainformat = (pam->format == PGM_FORMAT || pam->format == PPM_FORMAT) ? 1 : 0;
  pnm_writepaminit(pam);
  if (pam->format == PGM_FORMAT || pam->format == RPGM_FORMAT)
    write_pgm_pixel_data(img);
  else if (pam->format == PPM_FORMAT || pam->format == RPPM_FORMAT)
    write_ppm_pixel_data(img);
  else {
    fprintf(stderr, "error: file %s, line %d: shouldn’t be here!\n",__FILE__, __LINE__);
    exit(EXIT_FAILURE);
  }
  pm_close(pam->file);
}

void free_image(Image *img)
{
  if (img != NULL) {
    free_matrix((*img)->r);
    free_matrix((*img)->g);
    free_matrix((*img)->b);
    free(*img);
    *img = NULL;
  }
}
