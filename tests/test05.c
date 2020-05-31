#include <mcegraphics.h>
#include <array.h>

int main(int argc,char *argv[])
{
  double **m;
  int i, j;
  
  make_matrix(m,8,8);
  for(i = 0;i < 8;i++)
    for(j = 0; j < 8;j++)
      m[i][j] = i+j+1;
  
  printf("The matrix:\n");
  print_matrix("%10.6g\t",m,8,8);
  
  dct_matrix(m,8,8,DCT_FWD,DCT_SCALED);
  printf("\nThe scaled discrete cosine transform (Matlab):\n");
  print_matrix("%10.6g\t",m,8,8);
  
  dct_matrix(m,8,8,DCT_REV,DCT_SCALED);
  printf("\nThe restored matrix:\n");
  print_matrix("%10.6g\t",m,8,8);
  
  
  dct_matrix(m,8,8,DCT_FWD,DCT_UNSCALED);
  printf("\nThe unscaled discrete cosine transform:\n");
  print_matrix("%10.6g\t",m,8,8);
  
  dct_matrix(m,8,8,DCT_REV,DCT_UNSCALED);
  printf("\nThe unscaled restored matrix:\n");
  print_matrix("%10.6g\t",m,8,8);
  
  free_matrix(m);
  return 0;
}
