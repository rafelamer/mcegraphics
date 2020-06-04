#include <mcegraphics.h>

void transform(double **red,double **green,double **blue,void *params)
{
  Params p = (Params )params;
  int i, j;
  for (i = 0;i < p->size;i++)
    for(j = 0;j < p->size;j++)
      RGBToYCC(red[i] + j,green[i] + j,blue[i] + j);
}

int main(int argc,char *argv[])
{
  Image img;
  Params p;
 	p = (Params)malloc(sizeof(function_params));
	p->size = 256;
  pm_init(argv[0],0);
  img = read_image("tiger.ppm");
  foreach_submatrices(img->r,img->g,img->b,img->pam.height,img->pam.width,p->size,transform,(void *)p);

  write_image("tiger3.ppm",img);
  
  freeImage(img);

  return 0;
}
