#include <mce/mcegraphics.h>
#include <string.h>

void usage(int argc,char *argv[])
{
  if((argc < 2) || (argc > 3))
    {
      printf("Usage: %s infile [outfile]\n",argv[0]);
      printf("Reads a PGM or PPM file from infile\n");
      printf("If outfile, writes a identical image to outfile\n");
      exit(EXIT_FAILURE);
    }
  if(argc == 3 && strcmp(argv[1],argv[2]) == 0)
    {
      printf("Infile and outfile cannot be the same file\n");
      exit(EXIT_FAILURE);
    }
}


int main(int argc,char *argv[])
{
  usage(argc,argv);
  Image img;
  
  pm_init(argv[0],0);
  img = read_image(argv[1]);
  printf("Format: %d\n",img->pam.format);
  printf("Plain format: %d\n",img->pam.plainformat);
  printf("Width: %d\n",img->pam.width);
  printf("Height: %d\n",img->pam.height);
  printf("Depth: %d\n",img->pam.depth);
  printf("Maxval: %lu\n",img->pam.maxval);
  printf("Size: %u\n",img->pam.size);
  printf("Len: %u\n",img->pam.len);
  printf("Bytes per sample: %u\n",img->pam.bytes_per_sample);
  printf("Tuple type: %s\n",img->pam.tuple_type);
  
  if(argc == 3)
    write_image(argv[2],img);
  
  freeImage(img);

  return EXIT_SUCCESS;
}
