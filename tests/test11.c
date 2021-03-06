#include <mcegraphics.h>
#include <array.h>
#include <math.h>

int main(int argc,char *argv[])
{
  double **m, **r;
  double *aux;
  unsigned short **z;
  short *vec;
  size_t i, j, size;
  int len;
  unsigned char *out;

  size = 16;
  make_matrix(m,size,size);
  make_matrix(z,size,size);
  make_vector(vec,size * size);
  make_vector(aux,size * size);
  zigzag_matrix(z,size);
  make_vector(out,sizeof(short) * size * size + 1);

  /*
    Generate a matrix if integers values and clone it
   */
  for(i = 0;i < size;i++)
    for(j = 0; j < size;j++)
      m[i][j] = 1.0*((i * j * i * j + 10 * i + 20 * j) % 256);

  clone_matrix(m,r,size,size);

  /*
    Obtain his Haar transform
   */
  haar_transform_matrix(r,size,size,WT_FWD);
  
  /*
    Discard some elements in the matrix
   */
  prune_matrix_by_percentage(r,aux,size,size,90);
  
  /*
    Truncate each number to the rearest integer an
    store the values in a vector
   */
  for(i = 0;i < size;i++)
    for(j = 0;j < size;j++)
      vec[z[i][j] - 1] = r[i][j];

  /*
    Compress the values in the vector
   */
  wsq_compress_data(vec,out,size * size,&len);

  /*
    Uncompress the data
   */
  wsq_uncompress_data(vec,out,size * size,NULL);
  
  /*
    Restore the values to the matrix
   */  
  for(i = 0;i < size;i++)
    for(j = 0; j < size;j++)
      r[i][j] = vec[z[i][j] - 1];
  
  /*
    Reverse the DCT and aproximate each value of the nearest intger
   */
  haar_transform_matrix(r,size,size,WT_REV);
  for(i = 0;i < size;i++)
    for(j = 0; j < size;j++)
      r[i][j] = lrint(r[i][j]);

  /*
    Compares the initial matrix with the restored
  */
  for(i = 0;i < size;i++)
    {
      for(j = 0; j < size;j++)
	printf("%10.6g\t",r[i][j] - m[i][j]);
      printf("\n");
    }

  free_matrix(m);
  free_matrix(z);
  free_vector(vec);
  free_matrix(r);
  free_vector(out);
  free_vector(aux);
  
  return 0;
}
