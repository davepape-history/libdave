#include <stdio.h>
#include <gl.h>
#include "dave.h"

int dpInitTexmapping(void)
{
 float tevprops[] = { TV_NULL };
 if (!getgdesc(GD_TEXTURE)) return(0);
 tevdef(1,0,tevprops);
 tevbind(TV_ENV0,1);
 return(1);
}

int dpCreateTextureFromFile(char *fname)
{
 float texprops[] = { TX_NULL };
 unsigned long *img;
 int xres,yres,id;
 id = dpNewID();
 if (!dpReadSgiImageLong(fname,&img,&xres,&yres)) return(0);
 texdef2d(id,4,xres,yres,img,0,texprops);
 free(img);
 return id;
}

int dpCreateTexture(unsigned long *img,long xres,long yres)
{
 float texprops[] = { TX_NULL };
 int id = dpNewID();
 texdef2d(id,4,xres,yres,img,0,texprops);
 return id;
}
