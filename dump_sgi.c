#include <stdio.h>
#include <fcntl.h>
#include <gl.h>

void dpDumpSgi(char *name)
{
 long xsize,ysize;
 unsigned long *image;
 readsource(SRC_FRONT);
 getsize(&xsize,&ysize);
 image = (unsigned long *) malloc(xsize*ysize*sizeof(unsigned long));
 lrectread(0,0,xsize-1,ysize-1,image);
 if (!dpWriteSgiImageLong(name,image,xsize,ysize)) exit();
 free(image);
}

void dpDumpSgiStereo(char *basename)
{
 char name[80];
 long xsize,ysize;
 unsigned long *image;
 readsource(SRC_FRONTRIGHT);
 getsize(&xsize,&ysize);
 image = (unsigned long *) malloc(xsize*ysize*sizeof(unsigned long));
 lrectread(0,0,xsize-1,ysize-1,image);
 sprintf(name,"%s.right.sgi",basename);
 if (!dpWriteSgiImageLong(name,image,xsize,ysize)) { perror("writesgi"); return; }
 readsource(SRC_FRONTLEFT);
 lrectread(0,0,xsize-1,ysize-1,image);
 sprintf(name,"%s.left.sgi",basename);
 if (!dpWriteSgiImageLong(name,image,xsize,ysize)) { perror("writesgi"); return; }
 free(image);
}

void dpDumpSgiWithAlpha(char *basename)
{
 char name[80];
 long xsize,ysize,i,j;
 unsigned long *image,*ip;
 unsigned char *alpha,*ap;
 readsource(SRC_FRONT);
 getsize(&xsize,&ysize);
 image = (unsigned long *) malloc(xsize*ysize*sizeof(unsigned long));
 lrectread(0,0,xsize-1,ysize-1,image);
 sprintf(name,"%s.rgb",basename);
 if (!dpWriteSgiImageLong(name,image,xsize,ysize)) exit();
 alpha = (unsigned char *) malloc(xsize*ysize*sizeof(unsigned char));
 for (j=0, ip=image, ap=alpha; j<ysize; j++)
	for (i=0; i<xsize; i++, ip++, ap++)
		*ap = (*ip) >> 24;
 sprintf(name,"%s.alpha",basename);
 if (!dpWriteSgiImage(name,alpha,alpha,alpha,xsize,ysize,1)) exit();
 free(alpha);
 free(image);
}

void dpDumpSgiByte(char *name)
{
 long xsize,ysize,i;
 unsigned long *image,*ip;
 unsigned char *red,*rp;
 readsource(SRC_FRONT);
 getsize(&xsize,&ysize);
 image = (unsigned long *) malloc(xsize*ysize*sizeof(unsigned long));
 lrectread(0,0,xsize-1,ysize-1,image);
 red = (unsigned char *) malloc(xsize*ysize);
 for (i=xsize*ysize, ip=image, rp=red; i; i--, ip++, rp++)
	*rp = *ip & 0xff;
 if (!dpWriteSgiImageByte(name,red,xsize,ysize)) exit();
 free(red);
 free(image);
}

void dpDumpSgiBW(char *name)
{
 long xsize,ysize,i;
 unsigned long *image,*ip;
 unsigned char *bw,*bp;
 readsource(SRC_FRONT);
 getsize(&xsize,&ysize);
 image = (unsigned long *) malloc(xsize*ysize*sizeof(unsigned long));
 lrectread(0,0,xsize-1,ysize-1,image);
 bw = (unsigned char *) malloc(xsize*ysize);
 for (i=xsize*ysize, ip=image, bp=bw; i; i--, ip++, bp++)
	*bp = (*ip & 0xff)*0.3 + ((*ip & 0xff00)>>8)*0.59 +
		((*ip & 0xff0000)>>16)*0.11;
 if (!dpWriteSgiImageByte(name,bw,xsize,ysize)) exit();
 free(bw);
 free(image);
}
