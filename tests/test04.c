#include <mce/mcegraphics.h>
#include <mce/array.h>

int main(int argc,char *argv[])
{
  double *v;
  int i;
  
  make_vector(v,16);
  for(int i=0;i<16;i++)
    v[i] = 10+i*(i-20);
  
  printf("The vector:\n");
  print_vector("%10.6g\t",v,16);
  
  dct_vector(v,16,DCT_FWD,DCT_SCALED);
  printf("\nThe scaled discrete cosine transform (Matlab):\n");
  print_vector("%10.6g\t",v,16);
  
  dct_vector(v,16,DCT_REV,DCT_SCALED);
  printf("\nThe restored vector:\n");
  print_vector("%10.6g\t",v,16);
  
  
  dct_vector(v,16,DCT_FWD,DCT_UNSCALED);
  printf("\nThe unscaled discrete cosine transform:\n");
  print_vector("%10.6g\t",v,16);
  
  dct_vector(v,16,DCT_REV,DCT_UNSCALED);
  printf("\nThe unscaled restored vector:\n");
  print_vector("%10.6g\t",v,16);
  free_vector(v);
  
  return 0;
}
