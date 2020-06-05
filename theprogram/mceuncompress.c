#include <mcegraphics.h>
#include <array.h>
#include <math.h>
#include <stdlib.h>
#include <uncmdline.h>

void process_submatrix(double **m,void *params)
{
	Params p = (Params)params;
	read_and_uncompress(m,p->data,p->cdata,p->zigzag,p->size,p->compress,p->inout);
	if(p->transform == TRANSFORMDCT)
		dct_matrix(m,p->size,p->size,DCT_REV,DCT_SCALED);
	else 
		haar_transform_matrix(m,p->size,p->size,WT_REV);
}

void process_submatrices(double **red,double **green,double **blue,void *params)
{
	Params p = (Params)params;
	read_and_uncompress(red,p->data,p->cdata,p->zigzag,p->size,p->compress,p->inout);
	read_and_uncompress(green,p->data,p->cdata,p->zigzag,p->size,p->compress,p->inout);
	read_and_uncompress(blue,p->data,p->cdata,p->zigzag,p->size,p->compress,p->inout);
	if(p->transform == TRANSFORMDCT)
	{
		dct_matrix(red,p->size,p->size,DCT_REV,DCT_SCALED);
		dct_matrix(green,p->size,p->size,DCT_REV,DCT_SCALED);
		dct_matrix(blue,p->size,p->size,DCT_REV,DCT_SCALED);
	}
	else
	{
		haar_transform_matrix(red,p->size,p->size,WT_REV);
		haar_transform_matrix(green,p->size,p->size,WT_REV);
		haar_transform_matrix(blue,p->size,p->size,WT_REV);
	}
}

int main(int argc,char *argv[])
{
	Image img;
	Params p;

	static struct gengetopt_args_info args_info;

	/*
		Parse de command line parameters
	*/
	if (uncmdline_parser(argc,argv,&args_info) != 0) 
	{
    	uncmdline_parser_free(&args_info);
    	fprintf(stderr,"Error reading the command line parameters\n");
      	exit(EXIT_FAILURE);
  	}
	/*
		Setting the parameters for function process_submatrix
	*/
 	p = (Params)malloc(sizeof(function_params));
 	p->inout = fopen(args_info.infile_arg,"r");
	file_read(&p->height,sizeof(int),1,p->inout);
	file_read(&p->width,sizeof(int),1,p->inout);
	file_read(&p->depth,sizeof(int),1,p->inout);
	file_read(&p->size,sizeof(unsigned short),1,p->inout);
	file_read(&p->transform,sizeof(unsigned char),1,p->inout);
 	file_read(&p->compress,sizeof(unsigned char),1,p->inout);

 	make_vector(p->data,p->size*p->size);
 	make_vector(p->cdata,sizeof(short)*p->size*p->size+1);
 	make_matrix(p->zigzag,p->size,p->size);
 	zigzag_matrix(p->zigzag,p->size);
	p->action = RESTOREMATRIX;
 	/*
		Setting the image basic data
 	*/
 	pm_init(argv[0],0);
	img = (Image)malloc(sizeof(image_data));

	img->pam.width = p->width;
	img->pam.height = p->height;
	img->pam.depth = p->depth;
	img->pam.plainformat = 0;
	img->pam.maxval = 255;
	img->pam.len = 1024;
	img->pam.size = 1024;
	if(p->depth == 1)
	{
		img->pam.format = RPGM_FORMAT;
		sprintf(img->pam.tuple_type,"%s",PAM_PGM_TUPLETYPE);
	}
	else
	{
		img->pam.format = RPPM_FORMAT;
		sprintf(img->pam.tuple_type,"%s",PAM_PPM_TUPLETYPE);
	}
	make_matrix(img->r,p->height,p->width);
	make_matrix(img->g,p->height,p->width);
	make_matrix(img->b,p->height,p->width);

	if(img->pam.depth == 1)
	{
		foreach_submatrix(img->g,img->pam.height,img->pam.width,process_submatrix,(void *)p);
	}
	else
	{
		foreach_submatrices(img->r,img->g,img->b,img->pam.height,img->pam.width,process_submatrices,(void *)p);
	}
	write_image(args_info.outfile_arg,img);	
	uncmdline_parser_free(&args_info);
	free_vector(p->data);
	free_vector(p->cdata);
	free_matrix(p->zigzag);
	fclose(p->inout);
	free(p);
	free_image(&img);
}
