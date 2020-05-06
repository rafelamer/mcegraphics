#include <mce/mcegraphics.h>
#include <mce/array.h>

int main(int argc,char *argv[])
{
  double **m;
  int i, j;
  make_matrix(m,8,8);
  for(i = 0;i < 8;i++)
    for(j = 0;j < 8;j++)
      m[i][j] = i+j+1;
  
  printf("The matrix:\n");
  print_matrix("%10.6g\t",m,8,8);
  
  haar_transform_matrix(m,8,8,WT_FWD);
  printf("\nThe Haar transform:\n");
  print_matrix("%10.6g\t",m,8,8);
  
  haar_transform_matrix(m,8,8,WT_REV);
  printf("\nThe restored matrix:\n");
  print_matrix("%10.6g\t",m,8,8);
  
  free_matrix(m);
  return 0;
}
