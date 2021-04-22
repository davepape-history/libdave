#ifndef _dave_h_
#define _dave_h_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <math.h>
#include <gl/gl.h>

#define UCHAR unsigned char
#define ULONG unsigned long
#define UINT unsigned int

#define PALLOC(x) (x *) malloc(sizeof(x))
#define ARRALLOC(x,n) (x *) malloc((n)*sizeof(x))

#define MIN(x,y)	(((x) < (y)) ? (x) : (y))
#define MAX(x,y)	(((x) > (y)) ? (x) : (y))
#define CLAMP(v,min,max)  { if (v<(min)) v=(min); else if (v>(max)) v=(max); }

#if 0
#define SWAP(a,b)	a ^= b ^= a ^= b
#endif

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

unsigned char *dpReadBinaryFile(char *fname,UCHAR *data,int size);
int dpWriteBinaryFile(char *fname,UCHAR *data,int size);
void dpBeginTiming(void);
float dpEndTiming(int print_time);
void dpPrintMatrix(Matrix m);
int dpReadSgiImageHeader(char *fname,int *xdim,int *ydim,int *zdim,
		int *min,int *max,int *bpp,char *type,char *storage);
int dpReadSgiImage(char *fname,UCHAR **red,UCHAR **green,UCHAR **blue,
		int *x,int *y,int *z);
int dpWriteSgiImage(char *fname,UCHAR *red,UCHAR *green,UCHAR *blue,
		int x,int y,int z);
int dpReadSgiImageLong(char *fname,ULONG **data,int *xdim,int *ydim);
int dpWriteSgiImageLong(char *fname,ULONG *data,int xdim,int ydim);
int dpReadSgiImageByte(char *fname,UCHAR **red,int *xdim,int *ydim);
int dpWriteSgiImageByte(char *fname,UCHAR *red,int xdim,int ydim);
int dpReadSgiImageLongA(char *RGBname,char *Aname,ULONG **data,int *xdim,
		int *ydim);
char *dpGLDeviceName(int devnum);
void dpRGBtoHSV(int red,int green,int blue,int *hue,int *sat,int *val);
void dpHSVtoRGB(int hue,int sat,int val,int *red,int *green,int *blue);
void dpDumpSgi(char *name);
void dpDumpSgiWithAlpha(char *basename);
void dpDumpSgiByte(char *name);
void dpDumpSgiBW(char *name);
int dpNewID(void);
int dpInitTexmapping(void);
int dpCreateTextureFromFile(char *fname);
int dpCreateTexture(unsigned long *img,long xres,long yres);
void dpTTrigInit(void);
float dpTSin(float x);
float dpTCos(float x);
float dpTTan(float x);
int dpInvertAffineMatrix(Matrix in,Matrix out);


#ifdef __cplusplus
}
#endif

#endif
