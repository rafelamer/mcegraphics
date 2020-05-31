#include <mcegraphics.h>
#include <array.h>

int cmpfunc(const void *a, const void *b) {
  if(*(double *)a > *(double *)b)
    return 1;
  if(*(double *)a < *(double *)b)
    return -1;
  return 0;
}

int main(int argc,char *argv[])
{
  double **m, *v;
  int i, j;
    
  make_matrix(m,8,8);
  make_vector(v,8 * 8);
  for (i = 0; i < 8;i++)
    for (j = 0;j < 8;j++)
      m[i][j] = i+j+1;
  matrix_to_vector(m,v,8,8,double);
  qsort(v,64,sizeof(double),cmpfunc);
  
  printf("The matrix:\n");
  print_matrix("%10.6g",m,8,8);
  printf("\nThe vector:\n");
  print_vector("%10.6g",v,8 * 8);

  vector_to_matrix(v,m,8,8,double);
  printf("\nThe restored matrix:\n");
  print_matrix("%10.6g",m,8,8);

  free_matrix(m);
  free_vector(v);
  
  return EXIT_SUCCESS;
}
