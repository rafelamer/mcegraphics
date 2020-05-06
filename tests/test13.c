#include <mce/mcegraphics.h>

void transform(double **red,double **green,double **blue,int size,void *params)
{
  int i, j;
  for (i = 0;i < size;i++)
    for(j = 0;j < size;j++)
      RGBToYCC(red[i] + j,green[i] + j,blue[i] + j);
}

int main(int argc,char *argv[])
{
  Image img;
  
  pm_init(argv[0],0);
  img = read_image("tiger.ppm");

  foreach_submatrices(img,256,TRANSFORMMATRIX,transform,NULL);
  write_image("tiger3.ppm",img);
  
  freeImage(img);

  return 0;
}
