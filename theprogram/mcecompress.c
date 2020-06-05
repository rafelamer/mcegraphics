#include <mcegraphics.h>
#include <array.h>
#include <math.h>
#include <stdlib.h>
#include <cmdline.h>

void process_submatrix(double **m,void *params)
{
	Params p = (Params )params;
	
	if(p->transform == TRANSFORMDCT)
		dct_matrix(m,p->size,p->size,DCT_FWD,DCT_SCALED);
	else 
		haar_transform_matrix(m,p->size,p->size,WT_FWD);
	prune_matrix_by_percentage(m,p->vector,p->size,p->size,p->quality);
	compress_and_save(m,p->data,p->cdata,p->zigzag,p->size,p->compress,p->inout);
}

void process_submatrices(double **red,double **green,double **blue,void *params)
{
	Params p = (Params)params;
	if(p->transform == TRANSFORMDCT) 
	{
		dct_matrix(red,p->size,p->size,DCT_FWD,DCT_SCALED);
		dct_matrix(green,p->size,p->size,DCT_FWD,DCT_SCALED);
		dct_matrix(blue,p->size,p->size,DCT_FWD,DCT_SCALED);
	}
	else
	{
		haar_transform_matrix(red,p->size,p->size,WT_FWD);
		haar_transform_matrix(green,p->size,p->size,WT_FWD);
		haar_transform_matrix(blue,p->size,p->size,WT_FWD);
	}
	prune_matrices_by_percentage(red,green,blue,p->T,p->vector,p->size,p->size,p->quality);
	compress_and_save(red,p->data,p->cdata,p->zigzag,p->size,p->compress,p->inout);
	compress_and_save(green,p->data,p->cdata,p->zigzag,p->size,p->compress,p->inout);
	compress_and_save(blue,p->data,p->cdata,p->zigzag,p->size,p->compress,p->inout);
}

int main(int argc,char *argv[])
{
	Image img;
	Params p;

	static struct gengetopt_args_info args_info;

	/*
		Parse de command line parameters
	*/
	if (cmdline_parser(argc,argv,&args_info) != 0) 
	{
    	cmdline_parser_free(&args_info);
    	fprintf(stderr,"Error reading the command line parameters\n");
      	exit(EXIT_FAILURE);
  	}
  	if(args_info.help_given)
  	{
  		printf("%s\n",gengetopt_args_info_usage);
  		printf("%s\n",*gengetopt_args_info_help);
  		cmdline_parser_free(&args_info);
  		exit(EXIT_SUCCESS);
  	}
	if((args_info.quality_arg < 0.0) || (args_info.quality_arg > 100.0))
	{
		printf("The quality option must be in the interval [0,100]\n");
		cmdline_parser_free(&args_info);
  		exit(EXIT_FAILURE);
	}

	/*
		Read the image
	*/
	pm_init(argv[0],0);
	img = read_image(args_info.infile_arg);
	if(img == NULL)
	{
		printf("Error reading the image %s\n",args_info.infile_arg);
		cmdline_parser_free(&args_info);
  		exit(EXIT_FAILURE);
	}	
	/*
		Setting the parameters for function process_submatrix
	*/
 	p = (Params)malloc(sizeof(function_params));
 	p->size = (unsigned short)args_info.blocksize_arg;
	p->action = SAVEMATRIX;
 	unsigned short pw = 1;
 	while(pw < p->size)
 		pw *= 2;
 	p->size = pw;
 	if(p->size > 256)
 		p->size = 256;

 	p->T = NULL;
 	make_vector(p->data,p->size * p->size);
 	make_vector(p->vector,p->size * p->size);
 	make_vector(p->cdata,sizeof(short) * p->size * p->size+1);
 	p->quality = args_info.quality_arg;
 	p->inout = fopen(args_info.outfile_arg,"w");
 	make_matrix(p->zigzag,p->size,p->size);
 	zigzag_matrix(p->zigzag,p->size);
	p->transform = args_info.transform_arg;
	p->compress = args_info.compress_arg;
	cmdline_parser_free(&args_info);
	
	/*
		Start writing the output file
	*/
 	file_write(&img->pam.height,sizeof(int),1,p->inout);
 	file_write(&img->pam.width,sizeof(int),1,p->inout);
 	file_write(&img->pam.depth,sizeof(int),1,p->inout);
 	file_write(&p->size,sizeof(unsigned short),1,p->inout);
 	file_write(&p->transform,sizeof(unsigned char),1,p->inout);
 	file_write(&p->compress,sizeof(unsigned char),1,p->inout);

	if(img->pam.depth == 1)
	{
		foreach_submatrix(img->g,img->pam.height,img->pam.width,process_submatrix,(void *)p);
	}
	else
	{
		make_matrix(p->T,p->size,p->size);
		foreach_submatrices(img->r,img->g,img->b,img->pam.height,img->pam.width,process_submatrices,(void *)p);
		free_matrix(p->T);
	}
	free_vector(p->data);
	free_vector(p->vector);
	free_vector(p->cdata);
	free_matrix(p->zigzag);
	fclose(p->inout);
	free(p);
	free_image(&img);
}
