#ifndef __TARGA__H__
#define __TARGA__H__
#include <GL/gl.h>

#define TARGA_HEADER_SIZE 0x12
#define TARGA_UNCOMP_RGB_IMG 0x02
#define TARGA_UNCOMP_BW_IMG 0x03

// filename - nazwa pliku
// width - szerokoœæ obrazu
// height - wysokoœæ obrazu
// format - format danych obrazu
// type - format danych pikseli obrazu
// pixels - wskaŸnik na tablicê z danymi obrazu

GLboolean load_targa (const char *filename, GLsizei &width, GLsizei &height,GLenum &format, GLenum &type, GLvoid *&pixels)
{
 pixels = NULL;
 width = 0;
 height = 0;
 FILE *tga = fopen(filename,"rb");
 if (!tga) return GL_FALSE;
 unsigned char header [TARGA_HEADER_SIZE];
 fread (header,TARGA_HEADER_SIZE,1,tga);
 fseek (tga,header [0],SEEK_CUR);
 width = header [12] + (header [13] << 8);
 height = header [14] + (header [15] << 8);
 if (header [2]==TARGA_UNCOMP_RGB_IMG && header[16]== 24)
  {
   pixels=new unsigned char [width*height*3];
   fread((void*)pixels,width*height*3,1,tga);
   format=GL_BGR;
   type=GL_UNSIGNED_BYTE;
  } else
   if (header[2]==TARGA_UNCOMP_RGB_IMG && header[16]==32)
    {
     pixels = new unsigned char[width*height*4];
     fread((void*)pixels,width*height*4,1,tga);
     format=GL_BGRA;
     type=GL_UNSIGNED_BYTE;
    } else
     if (header[2]==TARGA_UNCOMP_BW_IMG && header[16]==8)
      {
       pixels=new unsigned char [width*height];
       fread((void*)pixels,width*height,1,tga);
       format=GL_LUMINANCE;
       type=GL_UNSIGNED_BYTE;
      } else return GL_FALSE;
 fclose (tga);
 return GL_TRUE;
}

// filename - nazwa pliku
// width - szerokoœæ obrazu
// height - wysokoœæ obrazu
// format - format danych obrazu
// type - format danych pikseli obrazu
// pixels - wskaŸnik na tablicê z danymi obrazu

GLboolean save_targa (const char *filename, GLsizei width, GLsizei height,GLenum format, GLenum type, GLvoid *pixels)
{
 unsigned char header [TARGA_HEADER_SIZE];

 if (format!=GL_BGR && format!=GL_BGRA && format!=GL_LUMINANCE) return GL_FALSE;
 if (type != GL_UNSIGNED_BYTE) return GL_FALSE;
 FILE *tga = fopen (filename,"wb");
 if (tga == NULL) return GL_FALSE;
 memset (header,0,TARGA_HEADER_SIZE);
 if (format==GL_BGR || format==GL_BGRA) header [2]=TARGA_UNCOMP_RGB_IMG;
  else if (format==GL_LUMINANCE) header[2]=TARGA_UNCOMP_BW_IMG;
 header[12]=(unsigned char)width;
 header[13]=(unsigned char)(width>>8);
 header[14]=(unsigned char)height;
 header[15]=(unsigned char)(height>>8);
 if (format==GL_BGRA) header[16]=32;
  else if (format==GL_BGR) header[16]=24;
        else if (format==GL_LUMINANCE) header[16]=8;
 fwrite (header,TARGA_HEADER_SIZE,1,tga);
 if (format==GL_BGRA) fwrite(pixels,width*height*4,1,tga);
  else if (format==GL_BGR) fwrite (pixels,width*height*3,1,tga);
        else if (format==GL_LUMINANCE) fwrite (pixels,width*height,1,tga);
 fclose (tga);
 return GL_TRUE;
}

#endif

