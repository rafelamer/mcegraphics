#include <mcegraphics.h>
#include <string.h>
#include <array.h>

void zigzag_matrix(unsigned short **m,int size)
{
  int r = 1;
  
  for (int z = 0; z < size; z++)
  {
    if (z % 2 == 0)
      for (int i = 0; i <= z; i++)
        m[z - i][i] = r++;
    else
      for (int i = 0; i <= z; i++)
        m[i][z - i] = r++;
  }
  for (int z = 1; z < size; z++)
  {
    if (z % 2 == size % 2)
      for (int i = 1; i <= size - z; i++)
        m[z + i - 1][size - i] = r++;
    else
      for (int i = 1; i <= size - z; i++)
        m[size - i][z + i - 1] = r++;
  }
}

void zlib_compress_data(short *in,unsigned char *out,int size,unsigned short *l)
{
  z_stream defstream;
  defstream.zalloc = Z_NULL;
  defstream.zfree = Z_NULL;
  defstream.opaque = Z_NULL;
  defstream.avail_in = size*sizeof(short);
  defstream.next_in = (Bytef *)in;
  defstream.avail_out = (uInt)(size*sizeof(short)+1);
  defstream.next_out = (Bytef *)out;

  deflateInit(&defstream, Z_BEST_COMPRESSION);
  deflate(&defstream, Z_FINISH);
  deflateEnd(&defstream);
  *l = strlen((const char *)out);
}

void zlib_uncompress_data(short *out,char *in,int size,FILE *stream)
{
  unsigned short l;
  file_read(&l,sizeof(unsigned short),1,stream);
  file_read(&in,sizeof(char),l,stream);

  z_stream infstream; 
  infstream.zalloc = Z_NULL;
  infstream.zfree = Z_NULL;
  infstream.opaque = Z_NULL;
  infstream.avail_in = (uInt)(strlen(in));
  infstream.next_in = (Bytef *)in;
  infstream.avail_out = (uInt)(size*size);
  infstream.next_out = (Bytef *)out;

  inflateInit(&infstream);
  inflate(&infstream, Z_NO_FLUSH);
  inflateEnd(&infstream);
}

void wsq_compress_data(short *in,unsigned char *out,int insize,int *outsize)
{
    int count = 0, pos = 0;;

    while(count < insize)
    {
      unsigned char p;
      short val;
      int zeros = 0;
      /*
          Coding of zeros
      */
      while((in[count] == 0) && (count < insize))
      {
        zeros++;
        count++;
      }
      if(zeros > 0)
      {
        if(zeros <= 100)
        {
          out[pos++] = zeros;
        }
        else if(zeros <= 255)
        {
          out[pos++] = 105;
          out[pos++] = zeros;   
        }
        else if(zeros <= 65535)
        {
          out[pos++] = 106;
          p = zeros/256;
          out[pos++]= p;
          out[pos++]= zeros-256*p;
        }
      }
      /*
          Coding of non zero values
      */
      if(count >= insize)
        break;
      val = in[count];
      if((val >= 75) && (val <= 255))
      {
        out[pos++] = 101;
        out[pos++] = val;
      }
      else if((val <= -74) && (val >= -255))
      {
        out[pos++] = 102;
        out[pos++] = -val; 
      }
      else if((val > 255) && (val <= 65535))
      {
        out[pos++] = 103;
        p = val/256;  
        out[pos++] = p;
        out[pos++] = val-256*p; 
      }
      else if((val < -255) && (val >= -65535))
      {
        out[pos++] = 104;
        val = -val;
        p = val/256;  
        out[pos++] = p;
        out[pos++] = val-256*p; 
      }
      else if((val >= -73) && (val <= 74))
      {
        out[pos++] = 180+val; 
      }
      count++;
    } 
    *outsize = pos;
}


void wsq_uncompress_data(short *out,int size,FILE *stream)
{
  int count = 0;
  while(count < size)
  {
    unsigned char p, q, r;
    file_read(&p,sizeof(char),1,stream);
    if((p > 0) && (p <= 100))
    {
      for(int i=0;i < p;i++)
        out[count+i] = 0;
      count += p;
    }
    else if(p == 105)
    {
      file_read(&q,sizeof(char),1,stream);
      for(int i=0;i < q;i++)
        out[count+i] = 0;
      count += q;
    }
    else if(p == 106)
    {
      file_read(&q,sizeof(char),1,stream);
      file_read(&r,sizeof(char),1,stream);
      short val = 256*q+r;
      for(int i=0;i < val;i++)
        out[count+i] = 0;
      count += val;
    }
    else if(p == 101)
    {
      file_read(&q,sizeof(char),1,stream);
      out[count] = q;
      count++;
    }
    else if(p == 102)
    {
      file_read(&q,sizeof(char),1,stream);
      out[count] = q;
      out[count] *= -1;
      count++;
    }
    else if(p == 103)
    {
      file_read(&q,sizeof(char),1,stream);
      file_read(&r,sizeof(char),1,stream);
      short val = 256*q+r;
      out[count] = val;
      count++;
    }
    else if(p == 104)
    {
      file_read(&q,sizeof(char),1,stream);
      file_read(&r,sizeof(char),1,stream);
      short val = 256*q+r;
      out[count] = val;
      out[count] *= -1;
      count++;
    }
    else
    {
      out[count] = p;
      out[count] -= 180;
      count++;
    }
  }
}
