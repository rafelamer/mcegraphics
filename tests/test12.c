#include <mce/mcegraphics.h>

void transform(double **red,double **green,double **blue,int size,void *params)
{
  int i, j;
  for (i = 0;i < size;i++)
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
  
  pm_init(argv[0],0);
  img = read_image("tiger.ppm");

  foreach_submatrices(img,256,TRANSFORMMATRIX,transform,NULL);
  write_image("tiger2.ppm",img);
  
  freeImage(img);

  return 0;
}
