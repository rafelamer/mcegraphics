#include <mcegraphics.h>
#include <array.h>

int main(int argc,char *argv[])
{
  double *v;
  make_vector(v,16);
  for(int i=0;i<16;i++)
    v[i] = 10+i*(i-20);
  
  printf("The vector:\n");
  print_vector("%10.6g\t",v,16);
  
  haar_transform_vector(v,16,WT_FWD);
  printf("\nThe Haar transform:\n");
  print_vector("%10.6g\t",v,16);
  
  haar_transform_vector(v,16,WT_REV);
  printf("\nThe restored vector:\n");
  print_vector("%10.6g\t",v,16);
  
  free_vector(v);
  
  return 0;
}
