#include <mcegraphics.h>
#include <string.h>
#include <array.h>

/****************************************************************************************
* The functions aux_zlib_compress_data and aux_zlib_uncompress_data are based on the file
* zpipe.c example of proper use of zlib's inflate() and deflate()
*
* zpipe.c:    Not copyrighted -- provided to the public domain
*             Version 1.4  11 December 2005  Mark Adler
*
*             See https://www.zlib.net/zlib_how.html
*****************************************************************************************/

#define ZLIBCHUNK 16384

#define CALL_ZLIB_DEFLATE(x)  unsigned char out[ZLIBCHUNK];							                \
	st.avail_in = (x) * sizeof(unsigned char);														                \
	st.next_in = (Bytef *)p;																							                \
	insize -= (x);																												                \
	flush = (insize == 0) ? Z_FINISH : Z_NO_FLUSH;												                \
	do																																		                \
	{																																			                \
		size_t have;																												                \
		st.avail_out = ZLIBCHUNK;																						                \
		st.next_out = (Bytef *)out;																					                \
		ret = deflate(&st,flush);																						                \
		if ((ret != Z_OK) && (ret != Z_STREAM_END))													                \
		{																																		                \
			deflateEnd(&st);																									                \
			free(str);																												                \
			*alloc = 0;																												                \
			*outsize = 0;																											                \
			return NULL;																											                \
		}																																		                \
		have = ZLIBCHUNK - st.avail_out;																		                \
		if ((*alloc - *outsize) < have)																			                \
		{																																		                \
			*alloc += ZLIBCHUNK;																							                \
			if ((str = (unsigned char *)realloc(str,*alloc * sizeof(unsigned char))) == NULL) \
				return NULL;																										                \
		}																																		                \
		memcpy(str + *outsize,out,have);																		                \
		*outsize += have;																										                \
	} while (st.avail_out == 0);																					                \
	p += (x);

#define CALL_ZLIB_INFLATE(x)  unsigned char out[ZLIBCHUNK];							                \
	st.avail_in = (x) * sizeof(unsigned char);														                \
	st.next_in = (Bytef *)p;																							                \
	insize -= (x);																												                \
	do																																		                \
	{																																			                \
		size_t have;																												                \
		st.avail_out = ZLIBCHUNK;																						                \
		st.next_out = (Bytef *)out;																					                \
		ret = inflate(&st,Z_NO_FLUSH);																			                \
		if ((ret != Z_OK)  && (ret != Z_STREAM_END))												                \
		{																																		                \
			inflateEnd(&st);																									                \
			free(str);																												                \
			*alloc = 0;																												                \
			*outsize = 0;																											                \
			return NULL;																											                \
		}																																		                \
		have = ZLIBCHUNK - st.avail_out;																		                \
		if ((*alloc - *outsize) < have)																			                \
		{																																		                \
			*alloc += ZLIBCHUNK;																							                \
			if ((str = (unsigned char *)realloc(str,*alloc * sizeof(unsigned char))) == NULL) \
				return NULL;																										                \
		}																																		                \
		memcpy(str + *outsize,out,have);																		                \
		*outsize += have;																										                \
	} while (st.avail_out == 0);																					                \
	p += (x);


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

unsigned char *aux_zlib_compress_data(unsigned char *data, size_t insize,size_t * outsize, size_t * alloc)
{
	z_stream st;
	unsigned char *str, *p;
	int flush, ret;

	*outsize = 0;
	*alloc = 0;
	if (insize == 0)
		return NULL;

	st.zalloc = Z_NULL;
	st.zfree = Z_NULL;
	st.opaque = Z_NULL;
	if (deflateInit(&st, Z_BEST_COMPRESSION) != Z_OK)
		return NULL;

	*alloc = 2 * ZLIBCHUNK;
	if ((str = (unsigned char *)calloc(*alloc, sizeof(unsigned char))) == NULL)
		return NULL;

	p = data;
	while (insize > ZLIBCHUNK)
	{
		CALL_ZLIB_DEFLATE(ZLIBCHUNK);
	}
	if (insize > 0)
	{
		CALL_ZLIB_DEFLATE(insize);
	}
	deflateEnd(&st);
	return str;
}

unsigned char *aux_zlib_uncompress_data(unsigned char *data, size_t insize,size_t * outsize, size_t * alloc)
{
	z_stream st;
	unsigned char *str, *p;
	int ret;

	*outsize = 0;
	*alloc = 0;
	if (insize == 0)
		return NULL;

	st.zalloc = Z_NULL;
	st.zfree = Z_NULL;
	st.opaque = Z_NULL;
	if (inflateInit(&st) != Z_OK)
		return NULL;

	*alloc = ZLIBCHUNK;
	if ((str = (unsigned char *)calloc(*alloc, sizeof(unsigned char))) == NULL)
		return NULL;

	p = data;
	while (insize > ZLIBCHUNK)
	{
		CALL_ZLIB_INFLATE(ZLIBCHUNK);
	}
	if (insize > 0)
	{
		CALL_ZLIB_INFLATE(insize);
	}
	inflateEnd(&st);
	return str;
}


void zlib_compress_data(short *in,unsigned char *out,int size,unsigned short *l)
{
  unsigned char *aux;
  size_t outsize, alloc;
  aux = aux_zlib_compress_data((unsigned char *)in,size,&outsize,&alloc);
  memcpy(out,aux,outsize);
  free(aux);
  *l = outsize;
}

void zlib_uncompress_data(short *out,char *in,int size)
{ 
  unsigned char *aux;
  size_t outsize, alloc;
  aux = aux_zlib_uncompress_data(in,size,&outsize,&alloc);
  memcpy(out,aux,outsize);
  free(aux);
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

#define read_one_byte(x) do {	             \
	if (stream == NULL)                      \
  {    												  	         \
		x = *aux;                              \
		aux++;                                 \
	}                                        \
  else                                     \
	  file_read(&x,sizeof(char),1,stream);   \
} while (0)

#define read_two_bytes(x,y) do {			     \
	if (stream == NULL)                      \
  {    												  	         \
		x = *aux;                              \
		aux++;                                 \
		y = *aux;                              \
		aux++;                                 \
	}                                        \
  else                                     \
  {    												  	         \
	  file_read(&x,sizeof(char),1,stream);   \
	  file_read(&y,sizeof(char),1,stream);   \
	}                                        \
} while (0)

void wsq_uncompress_data(short *out,unsigned char *in,int size,FILE *stream)
{
  int count = 0;
	unsigned char *aux;
	aux = in;
	
  while(count < size)
  {
    unsigned char p, q, r;
	  read_one_byte(p);	  
    if((p > 0) && (p <= 100))
    {
      for(int i=0;i < p;i++)
        out[count+i] = 0;
      count += p;
    }
    else if(p == 105)
    {
			read_one_byte(q);
      for(int i=0;i < q;i++)
        out[count+i] = 0;
      count += q;
    }
    else if(p == 106)
    {
			read_two_bytes(q,r);
      short val = 256*q+r;
      for(int i=0;i < val;i++)
        out[count+i] = 0;
      count += val;
    }
    else if(p == 101)
    {
			read_one_byte(q);
      out[count] = q;
      count++;
    }
    else if(p == 102)
    {
			read_one_byte(q);
      out[count] = q;
      out[count] *= -1;
      count++;
    }
    else if(p == 103)
    {
			read_two_bytes(q,r);
      short val = 256*q+r;
      out[count] = val;
      count++;
    }
    else if(p == 104)
    {
      read_two_bytes(q,r);
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
