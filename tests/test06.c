#include <mce/mcegraphics.h>
#include <mce/array.h>

int main(int argc,char *argv[])
{
  double *v;
  int i, j;
  make_vector(v,10);
  for(i = 0;i < 10;i++)
    v[i] = 3.0/(i+1);

  printf("The vector:\n");
  print_vector("%10.6g\t",v,10);
  free_vector(v);
  
  double **m;
  make_matrix(m,5,4);
  for(i = 0;i < 5;i++)
    for(j = 0;j < 4;j++)	
      m[i][j] = 2.0/(i+j+1);
  
  printf("\nThe matrix:\n");	
  print_matrix("%10.6g\t",m,5,4);
  free_matrix(m);
  
  return 0;
}
