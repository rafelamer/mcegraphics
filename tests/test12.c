#include <mcegraphics.h>

void transform(double **red,double **green,double **blue,void *params)
{
  int i, j;
	Params p = (Params )params;

  for (i = 0;i < p->size;i++)
    for(j = 0;j < i;j++)
		{
			double a;
			a = red[i][j];
			red[i][j] = red[j][i];
			red[j][i] = a;
			a = green[i][j];
			green[i][j] = green[j][i];
			green[j][i] = a;
			a = blue[i][j];
			blue[i][j] = blue[j][i];
			blue[j][i] = a;
		}
}

int main(int argc,char *argv[])
{
  Image img;
	Params p;
 	p = (Params)malloc(sizeof(function_params));
	p->size = 256;


	
  
  pm_init(argv[0],0);
  img = read_image("tiger.ppm");

  foreach_submatrices(img->r,img->g,img->b,img->pam.height,img->pam.width,p->size,TRANSFORMMATRIX,transform,(void *)p);

	write_image("tiger2.ppm",img);
  
  freeImage(img);
	free(p);
  return 0;
}
