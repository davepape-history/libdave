#include <stdio.h>
#include <fcntl.h>
#include <gl/gl.h>
#include "dave.h"

void dpDumpShrunk(char *name)
{
 long xsize,ysize,i,j,red,green,blue;
 unsigned long *image,*ip1,*ip2;
 unsigned char *r,*g,*b,*rp,*gp,*bp;
 readsource(SRC_FRONT);
 getsize(&xsize,&ysize);
 image = (unsigned long *) malloc(xsize*ysize*sizeof(long));
 r = (unsigned char *) malloc(xsize*ysize/4);
 g = (unsigned char *) malloc(xsize*ysize/4);
 b = (unsigned char *) malloc(xsize*ysize/4);
 lrectread(0,0,xsize-1,ysize-1,image);
 for (j=0, ip1=image, ip2=image+xsize, rp=r, gp=g, bp=b; j<ysize/2;
		j++, ip1+=xsize, ip2+=xsize)
   for (i=0; i<xsize/2; i++)
	{
	red = (*ip1 & 0xff) + (*ip2 & 0xff);
	green = ((*ip1 & 0xff00) >> 8) + ((*ip2 & 0xff00) >> 8);
	blue = ((*ip1 & 0xff0000) >> 16) + ((*ip2 & 0xff0000) >> 16);
	ip1++;  ip2++;
	red += (*ip1 & 0xff) + (*ip2 & 0xff);
	green += ((*ip1 & 0xff00) >> 8) + ((*ip2 & 0xff00) >> 8);
	blue += ((*ip1 & 0xff0000) >> 16) + ((*ip2 & 0xff0000) >> 16);
	ip1++;  ip2++;
	*rp++ = red/4;
	*gp++ = green/4;
	*bp++ = blue/4;
	}
 if (!dpWriteSgiImage(name,r,g,b,xsize/2,ysize/2,3)) exit();
 free(image);
 free(r);  free(g);  free(b);
}

void dpDumpShrunkWithAlpha(char *basename)
{
 long xsize,ysize,i,j,red,green,blue,alpha;
 unsigned long *image,*ip1,*ip2;
 unsigned char *r,*g,*b,*a,*rp,*gp,*bp,*ap;
 char fname[256];
 readsource(SRC_FRONT);
 getsize(&xsize,&ysize);
 image = (unsigned long *) malloc(xsize*ysize*sizeof(long));
 r = (unsigned char *) malloc(xsize*ysize/4);
 g = (unsigned char *) malloc(xsize*ysize/4);
 b = (unsigned char *) malloc(xsize*ysize/4);
 a = (unsigned char *) malloc(xsize*ysize/4);
 lrectread(0,0,xsize-1,ysize-1,image);
 for (j=0, ip1=image, ip2=image+xsize, rp=r, gp=g, bp=b, ap=a; j<ysize/2;
		j++, ip1+=xsize, ip2+=xsize)
   for (i=0; i<xsize/2; i++)
	{
	red = (*ip1 & 0xff) + (*ip2 & 0xff);
	green = ((*ip1 & 0xff00) >> 8) + ((*ip2 & 0xff00) >> 8);
	blue = ((*ip1 & 0xff0000) >> 16) + ((*ip2 & 0xff0000) >> 16);
	alpha = ((*ip1 & 0xff000000) >> 24) + ((*ip2 & 0xff000000) >> 24);
	ip1++;  ip2++;
	red += (*ip1 & 0xff) + (*ip2 & 0xff);
	green += ((*ip1 & 0xff00) >> 8) + ((*ip2 & 0xff00) >> 8);
	blue += ((*ip1 & 0xff0000) >> 16) + ((*ip2 & 0xff0000) >> 16);
	alpha += ((*ip1 & 0xff000000) >> 24) + ((*ip2 & 0xff000000) >> 24);
	ip1++;  ip2++;
	*rp++ = red/4;
	*gp++ = green/4;
	*bp++ = blue/4;
	*ap++ = alpha/4;
	}
 sprintf(fname,"%s.rgb",basename);
 if (!dpWriteSgiImage(fname,r,g,b,xsize/2,ysize/2,3)) exit();
 sprintf(fname,"%s.alpha",basename);
 if (!dpWriteSgiImageByte(fname,a,xsize/2,ysize/2)) exit();
 free(image);
 free(r);  free(g);  free(b);  free(a);
}
