#include <stdio.h>
#include <gl/gl.h>
#include "image.h"
#include "dave.h"

int dpReadSgiImageHeader(char *fname,int *xdim,int *ydim,int *zdim,
			int *min,int *max,int *bpp,char *type,char *storage)
{
 register IMAGE *image;
 if ((image=iopen(fname,"r")) == NULL )
	{
	fprintf(stderr,"dpReadSgiImageHeader: can't open input file %s\n",
			fname);
	return(0);
	}
 if (xdim) *xdim = image->xsize;
 if (ydim) *ydim = image->ysize;
 if (zdim) *zdim = image->zsize;
 if (min) *min = image->min;
 if (max) *max = image->max;
 if (bpp) *bpp = BPP(image->type);
 if (type)
	switch (image->colormap)
		{
		case CM_NORMAL: strcpy(type,"NORMAL"); break;
		case CM_DITHERED: strcpy(type,"DITHERED"); break;
		case CM_SCREEN: strcpy(type,"SCREEN"); break;
		case CM_COLORMAP: strcpy(type,"COLORMAP"); break;
		}
 if (storage)
	{
	if (ISRLE(image->type)) strcpy(storage,"rle");
	else strcpy(storage,"verb");
	}
 iclose(image);
 return(1);
}

int dpReadSgiImage(char *fname,UCHAR **red,UCHAR **green,UCHAR **blue,
		int *xdim,int *ydim,int *zdim)
{
 IMAGE *image;
 int x,y;
 UCHAR *r,*g,*b;
 short *buf;
 if ((image=iopen(fname,"r")) == NULL)
	{
	fprintf(stderr,"dpReadSgiImage: can't open input file %s\n",fname);
	return(0);
	}
 if (xdim) *xdim = image->xsize;
 if (ydim) *ydim = image->ysize;
 if (zdim) *zdim = image->zsize;
 buf = (short *) malloc(image->xsize*sizeof(short));
 if (red)
	r = *red = (UCHAR *) malloc(image->xsize*image->ysize);
 if ((green) && (image->zsize>1))
	g = *green = (UCHAR *) malloc(image->xsize*image->ysize);
 if ((blue) && (image->zsize>2))
	b = *blue = (UCHAR *) malloc(image->xsize*image->ysize);
 for (y=0; y<image->ysize; y++)
	{
	getrow(image,buf,y,0);
	if (red)
		for (x=0; x<image->xsize; x++) *r++ = buf[x];
	if ((green) && (image->zsize>1))
		{
		getrow(image,buf,y,1);
		for (x=0; x<image->xsize; x++) *g++ = buf[x];
		}
	if ((blue) && (image->zsize>2))
		{
		getrow(image,buf,y,2);
		for (x=0; x<image->xsize; x++) *b++ = buf[x];
		}
	}
 free(buf);
 iclose(image);
 return(1);
}

int dpReadSgiImageByte(char *fname,UCHAR **red,int *xdim,int *ydim)
{
 register IMAGE *image;
 register int x,y;
 UCHAR *r;
 short *buf;
 if ((image=iopen(fname,"r")) == NULL)
	{
	fprintf(stderr,"dpReadSgiImageByte: can't open input file %s\n",
		fname);
	return(0);
	}
 if (xdim) *xdim = image->xsize;
 if (ydim) *ydim = image->ysize;
 if (red)
	{
	buf = (short *) malloc(image->xsize*sizeof(short));
	r = *red = (UCHAR *) malloc(image->xsize*image->ysize);
	for (y=0; y<image->ysize; y++)
		{
		getrow(image,buf,y,0);
		for (x=0; x<image->xsize; x++) *r++ = buf[x];
		}
	free(buf);
	}
 iclose(image);
 return(1);
}

int dpReadSgiImageLong(char *fname,ULONG **data,int *xdim,int *ydim)
{
 register IMAGE *image;
 register int x,y;
 short *rbuf,*gbuf,*bbuf;
 ULONG *p;
 int zdim;
 if ((image=iopen(fname,"r")) == NULL ) {
	fprintf(stderr,"dpReadSgiImageLong: can't open input file %s\n",
		fname);
	return(0);
	}
 if (xdim) *xdim = image->xsize;
 if (ydim) *ydim = image->ysize;
 zdim = image->zsize;
 if (data)
	{
	rbuf = (short *) malloc(image->xsize*sizeof(short));
	if (zdim>1) gbuf = (short *) malloc(image->xsize*sizeof(short));
	else gbuf = rbuf;
	if (zdim>2) bbuf = (short *) malloc(image->xsize*sizeof(short));
	else bbuf = gbuf;
	p = *data = (ULONG *) malloc(image->xsize*image->ysize*sizeof(ULONG));
	for (y=0; y<image->ysize; y++)
		{
		getrow(image,rbuf,y,0);
		if (zdim > 1) getrow(image,gbuf,y,1);
		if (zdim > 2) getrow(image,bbuf,y,2);
		for (x=0; x<image->xsize; x++)
			*p++ = rbuf[x] | (gbuf[x]<<8) | (bbuf[x]<<16) | 0xff000000;
		}
	free(rbuf);
	if (zdim>1) free(gbuf);
	if (zdim>2) free(bbuf);
	}
 iclose(image);
 return(1);
}

int dpReadSgiImageLongA(char *RGBname,char *Aname,ULONG **data,int *xdim,
			int *ydim)
{
 IMAGE *RGBimage,*Aimage;
 int x,y;
 short *rbuf,*gbuf,*bbuf,*abuf;
 ULONG *p;
 int zdim;
 if ((RGBimage=iopen(RGBname,"r")) == NULL)
	{
	fprintf(stderr,"dpReadSgiImageLongA: can't open input file %s\n",
		RGBname);
	return(0);
	}
 if ((Aimage=iopen(Aname,"r")) == NULL)
	{
	iclose(RGBimage);
	fprintf(stderr,"dpReadSgiImageLongA: can't open input file %s\n",
		Aname);
	return(0);
	}
 if ((RGBimage->xsize != Aimage->xsize) || (RGBimage->ysize != Aimage->ysize))
	{
	iclose(RGBimage);
	iclose(Aimage);
	fprintf(stderr,"ERROR: dpReadSgiImageLongA: "
		"%s and %s are different sizes\n",RGBname,Aname);
	return 0;
	}
 if (xdim) *xdim = RGBimage->xsize;
 if (ydim) *ydim = RGBimage->ysize;
 zdim = RGBimage->zsize;
 if (data)
	{
	rbuf = (short *) malloc(RGBimage->xsize*sizeof(short));
	if (zdim>1) gbuf = (short *) malloc(RGBimage->xsize*sizeof(short));
	else gbuf = rbuf;
	if (zdim>2) bbuf = (short *) malloc(RGBimage->xsize*sizeof(short));
	else bbuf = gbuf;
	abuf = (short *) malloc(RGBimage->xsize*sizeof(short));
	p = *data = (ULONG *) malloc(RGBimage->xsize*RGBimage->ysize*sizeof(ULONG));
	for (y=0; y<RGBimage->ysize; y++)
		{
		getrow(RGBimage,rbuf,y,0);
		if (zdim > 1) getrow(RGBimage,gbuf,y,1);
		if (zdim > 2) getrow(RGBimage,bbuf,y,2);
		getrow(Aimage,abuf,y,0);
		for (x=0; x<RGBimage->xsize; x++)
			*p++ = rbuf[x] | (gbuf[x]<<8) | (bbuf[x]<<16) | (abuf[x]<<24);
		}
	free(rbuf);
	if (zdim>1) free(gbuf);
	if (zdim>2) free(bbuf);
	free(abuf);
	}
 iclose(RGBimage);
 iclose(Aimage);
 return 1;
}

int dpWriteSgiImage(char *fname,UCHAR *red,UCHAR *green,UCHAR *blue,
		int xdim,int ydim,int zdim)
{
 int i,y;
 unsigned short *sbuf;
 IMAGE *image;
 UCHAR *rp,*gp,*bp;
 sbuf = (unsigned short *) malloc(xdim*2);
 if (!(image = iopen(fname,"w",RLE(1),3,xdim,ydim,zdim)))
	{
	fprintf(stderr,"dpWriteSgiImage: can't open output file %s\n",
		fname);
	return(0);
	}
 for (y=0, rp=red, gp=green, bp=blue; y<ydim; y++)
	{
	for (i=0; i<xdim; i++) sbuf[i] = *rp++;
	putrow(image,sbuf,y,0);
	if (zdim>1)
		{
		for (i=0; i<xdim; i++) sbuf[i] = *gp++;
		putrow(image,sbuf,y,1);
		}
	if (zdim>2)
		{
		for (i=0; i<xdim; i++) sbuf[i] = *bp++;
		putrow(image,sbuf,y,2);
		}
	}
 iclose(image);
 free(sbuf);
 return 1;
}

int dpWriteSgiImageByte(char *fname,UCHAR *red,int xdim,int ydim)
{
 int i,y;
 unsigned short *sbuf;
 IMAGE *image;
 UCHAR *rp;
 sbuf = (unsigned short *) malloc(xdim*2);
 if (!(image = iopen(fname,"w",RLE(1),3,xdim,ydim,1)))
	{
	fprintf(stderr,"dpWriteSgiImageByte: can't open output file %s\n",
		fname);
	return(0);
	}
 for (y=0, rp=red; y<ydim; y++)
	{
	for (i=0; i<xdim; i++) sbuf[i] = *rp++;
	putrow(image,sbuf,y,0);
	}
 iclose(image);
 free(sbuf);
 return 1;
}

int dpWriteSgiImageLong(char *fname,ULONG *data,int xdim,int ydim)
{
 int i,y;
 unsigned short *sbuf;
 IMAGE *image;
 ULONG *p,*p1;
 sbuf = (unsigned short *) malloc(xdim*2);
 if (!(image = iopen(fname,"w",RLE(1),3,xdim,ydim,3)))
	{
	fprintf(stderr,"dpWriteSgiImageLong: can't open output file %s\n",
		fname);
	return(0);
	}
 for (y=0, p=data; y<ydim; y++, p+=xdim)
	{
	for (i=0, p1=p; i<xdim; i++) sbuf[i] = (*p1++) & 0xff;
	putrow(image,sbuf,y,0);
	for (i=0, p1=p; i<xdim; i++) sbuf[i] = ((*p1++) & 0xff00) >> 8;
	putrow(image,sbuf,y,1);
	for (i=0, p1=p; i<xdim; i++) sbuf[i] = ((*p1++) & 0xff0000) >> 16;
	putrow(image,sbuf,y,2);
	}
 iclose(image);
 free(sbuf);
 return 1;
}

void dpPrintMatrix(Matrix m)
{
 int j;
 for (j=0; j<4; j++)
	printf("[ %f %f %f %f ]\n",m[0][j],m[1][j],m[2][j],m[3][j]);
}

#include "Devices.h"

char *dpGLDeviceName(int devnum)
{
 int i;
 for (i=0; i<NUMDEVS; i++)
	if (devlist[i].number == devnum) return(devlist[i].name);
 return(NULL);
}
