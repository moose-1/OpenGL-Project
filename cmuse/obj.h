#ifndef __OBJ__H__
#define __OBJ__H__

//#include "openglut.h"
#include <stdio.h>
#include <string.h>
#include <vector>
//#include "obj.h"
//#include "targa.h"

struct geometric_vertices
{
 GLfloat x,y,z,w;
};

struct texture_vertices
{
 GLfloat u,v,w;
};

struct vertex_normals
{
 GLfloat i,j,k;
};

struct parameter_space_vertices
{
 GLfloat u,v,w;
};

std::vector <geometric_vertices> v;
std::vector <texture_vertices> vt;
std::vector <vertex_normals> vn;
std::vector <parameter_space_vertices> vp;

struct material
{
 char name[256];
 GLfloat Ka[4];
 GLfloat Kd[4];
 GLfloat Ks[4];
 int illum;
 GLfloat d;
 GLfloat Ns;
 char map_Kd[256];
};

std::vector <material> materials;

void DeafultMaterial (material &mat)
{
 mat.name[0]=0;
 mat.Ka[0]=0.2f;
 mat.Ka[1]=0.2f;
 mat.Ka[2]=0.2f;
 mat.Ka[3]=1.0f;
 mat.Kd[0]=0.8f;
 mat.Kd[1]=0.8f;
 mat.Kd[2]=0.8f;
 mat.Kd[3]=1.0f;
 mat.Ks[0]=1.0f;
 mat.Ks[1]=1.0f;
 mat.Ks[2]=1.0f;
 mat.Ks[3]=1.0f;
 mat.illum=1;
 mat.d=1.0;
 mat.Ns=0.0;
 mat.map_Kd[0]=0;
}

void load_vertex_data (const char *str)
{
 switch (str [1])
  {
   case ' ':
    geometric_vertices tmp_v;
    tmp_v.w=1.0;
    sscanf (str+1,"%f %f %f %f",&tmp_v.x,&tmp_v.y,&tmp_v.z,&tmp_v.w);
    v.insert(v.end(),tmp_v);
   break;
   case 't':
    texture_vertices tmp_vt;
    tmp_vt.v = tmp_vt.w = 0.0;
    sscanf (str+2,"%f %f %f",&tmp_vt.u,&tmp_vt.v,&tmp_vt.w);
    vt.insert (vt.end (),tmp_vt);
   break;
   case 'n':
    vertex_normals tmp_vn;
    sscanf (str+2,"%f %f %f",&tmp_vn.i,&tmp_vn.j,&tmp_vn.k);
    vn.insert (vn.end(),tmp_vn);
   break;
   case 'p':
    parameter_space_vertices tmp_vp;
    tmp_vp.w = 1.0;
    sscanf (str+2,"%f %f %f",&tmp_vt.u,&tmp_vt.v,&tmp_vt.w);
    vp.insert (vp.end (),tmp_vp);
   break;
  }
}

void load_materials(const char *str)
{
 char tmpstr [512];
 unsigned int pos = 0;
 sscanf (str,"%s",tmpstr);
 if (!strcmp(tmpstr,"mtllib"))
  {
   pos+=strlen(tmpstr) + 1;
   while (pos<strlen(str))
    {
     sscanf (str+pos,"%s",tmpstr);
     pos += strlen (tmpstr) + 1;
     FILE *mtl = fopen (tmpstr,"rt");
     if (!mtl) continue;
     material mat;
     DeafultMaterial (mat);
     char mtlstr [512];
     while (!feof(mtl))
      {
       fgets (mtlstr,512,mtl);
       switch (mtlstr[0])
        {
         case 'n':
          sscanf (mtlstr,"%s",tmpstr);
          if (!strcmp(tmpstr,"newmtl"))
           {
            if (strlen(mat.name))
             {
              materials.insert(materials.end (),mat);
              DeafultMaterial(mat);
             }
            sscanf (mtlstr+strlen(tmpstr)+1,"%s",mat.name);
           }
         break;
         case 'K':
          switch (mtlstr[1])
           {
            case 'a':
             sscanf (mtlstr+3,"%f %f %f",mat.Ka+0,mat.Ka+1,mat.Ka+2);
            break;
            case 'd':
             sscanf (mtlstr+3,"%f %f %f",mat.Kd+0,mat.Kd+1,mat.Kd+2);
            break;
            case 's':
             sscanf (mtlstr+3,"%f %f %f",mat.Ks+0,mat.Ks+1,mat.Ks+2);
            break;
           }
          break;
          case 'd':
           if (mtlstr [1] == ' ')
            {
             sscanf (mtlstr+2,"%f",&mat.d);
             mat.Ka [3] = mat.d;
             mat.Kd [3] = mat.d;
             mat.Ks [3] = mat.d;
            }
          break;
          case 'N':
           if (mtlstr [1] == 's' && mtlstr [2] == ' ')
            {
             sscanf (mtlstr+3,"%f",&mat.Ns);
             mat.Ns /= 1000.0;
             mat.Ns *= 128.0;
            }
          break;
          case 'm':
           sscanf (mtlstr,"%s",tmpstr);
           if (!strcmp (tmpstr,"map_Kd")) sscanf (mtlstr+strlen (tmpstr)+1,"%s",mat.map_Kd);
          break;
          case 'i':
           sscanf (mtlstr,"%s",tmpstr);
            if (!strcmp (tmpstr,"illum")) sscanf (mtlstr+strlen (tmpstr)+1,"%i",&mat.illum);
          break;
          default: break;
         }
        }
      materials.insert (materials.end (),mat);
      fclose (mtl);
    }
  }
}

void load_face (const char *str)
{
 char tmpstr [512];
 unsigned int pos = 2;
 sscanf (str+pos,"%s",tmpstr);
 unsigned int count = 0;
 for (unsigned int i = 0; i < strlen (tmpstr); i++)
  if (tmpstr [i] == '/')
   {
    count++;
    if (tmpstr [i+1] == '/') count++;
   }

 int vertex = 0;
 while (pos < strlen (str))
  {
   vertex++;
   pos += strlen (tmpstr) + 1;
   sscanf (str+pos,"%s",tmpstr);
  }
 switch (vertex)
  {
   case 3:
    glBegin (GL_TRIANGLES);
   break;
   case 4:
    glBegin (GL_QUADS);
   break;
   default:
    glBegin (GL_POLYGON);
   break;
  };

 pos = 2;
 vertex = 0;
 int iv,ivt,ivn;
 sscanf (str+pos,"%s",tmpstr);
 while (pos < strlen (str))
  {
   switch (count)
    {
     case 0:
      sscanf (tmpstr,"%i",&iv);
      glVertex3f (v[iv-1].x,v[iv-1].y,v[iv-1].z);
     break;
     case 1:
      sscanf (tmpstr,"%i/%i",&iv,&ivt);
      glTexCoord3f (vt[ivt-1].u,vt[ivt-1].v,vt[ivt-1].w);
      glVertex3f (v[iv-1].x,v[iv-1].y,v[iv-1].z);
     break;
     case 2:
      sscanf (tmpstr,"%i/%i/%i",&iv,&ivt,&ivn);
      glNormal3f (vn[ivn-1].i,vn[ivn-1].j,vn[ivn-1].k);
      glTexCoord3f (vt[ivt-1].u,vt[ivt-1].v,vt[ivt-1].w);
      glVertex3f (v[iv-1].x,v[iv-1].y,v[iv-1].z);
     break;
     case 3:
      sscanf (tmpstr,"%i//%i",&iv,&ivn);
      glNormal3f (vn[ivn-1].i,vn[ivn-1].j,vn[ivn-1].k);
      glVertex3f (v[iv-1].x,v[iv-1].y,v[iv-1].z);
     break;
    }
   vertex++;
   pos += strlen (tmpstr) + 1;
   sscanf (str+pos,"%s",tmpstr);
  }
 glEnd ();
}

GLboolean load_texture (const char *filename,GLuint *texture,GLuint &tex_num)
{
 GLsizei width, height;
 GLenum format, type;
 GLvoid *pixels;
 glPixelStorei (GL_UNPACK_ALIGNMENT,1);
 GLboolean error = load_targa (filename,width,height,format,type,pixels);
 if (error == GL_FALSE)
  {
   delete [] (unsigned char*)pixels;
   return GL_FALSE;
  }
 //extern GLuint texture[2];
 glBindTexture( GL_TEXTURE_2D, texture[tex_num] );
 tex_num++;
 gluBuild2DMipmaps (GL_TEXTURE_2D,GL_RGB,width,height,format,type,pixels);
 glTexEnvf (GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
 glTexParameteri (GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
 glTexParameteri (GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
 delete [] (unsigned char*)pixels;
 return GL_TRUE;
}

// filename - nazwa pliku
// object_list - numer listy wyœwietlania

GLboolean load_obj (const char *filename, GLuint &object_list, GLuint *texture,GLuint &tex_num)
{
 FILE *obj = fopen (filename,"rt");
 if (!obj) return GL_FALSE;
 object_list = glGenLists (1);
 glNewList (object_list,GL_COMPILE);
 char str [512];
 while (!feof (obj))
  {
   fgets (str,512,obj);
   switch (str[0])
    {
     case 'v':
      load_vertex_data (str);
     break;
     case 'f':
      load_face (str);
     break;
     case 'm':
      load_materials (str);
     break;
     case 'u':
      char name [128];
      sscanf (str,"%s",name);
      if (!strcmp (name,"usemtl"))
       {
        sscanf (str+7,"%s",name);
        for (unsigned int i = 0; i < materials.size (); i++)
         if (!strcmp (materials[i].name,name))
          {
           glDisable (GL_TEXTURE_2D);
           glDisable (GL_BLEND);
           glMaterialfv (GL_FRONT_AND_BACK,GL_AMBIENT,materials[i].Ka);
           glMaterialfv (GL_FRONT_AND_BACK,GL_DIFFUSE,materials[i].Kd);
           if (materials[i].illum == 2) glMaterialfv (GL_FRONT_AND_BACK,GL_SPECULAR,materials[i].Ks);
           glMaterialf (GL_FRONT_AND_BACK,GL_SHININESS,materials[i].Ns);
           if (materials[i].d < 1.0)
            {
             glEnable (GL_BLEND);
             glBlendFunc (GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
            }
           if (materials[i].map_Kd [0])
            {
             if (load_texture (materials[i].map_Kd,texture,tex_num)) glEnable (GL_TEXTURE_2D);
            }
           break;
         }
       }
      break;
      default: break;
     }
    }
 glEndList ();
 v.erase (v.begin (),v.end ());
 vt.erase (vt.begin (),vt.end ());
 vn.erase (vn.begin (),vn.end ());
 vp.erase (vp.begin (),vp.end ());
 materials.erase (materials.begin (),materials.end ());
 fclose (obj);
 return GL_TRUE;
}
#endif
