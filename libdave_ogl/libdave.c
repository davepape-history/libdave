#include <stdio.h>
#include <fcntl.h>
#include "dave.h"

/* dpNewID - returns a new ('unique') ID number. For materials, texmaps, etc. */
int dpNewID(void)
{
 static int next_number = 256;
 next_number++;
 return(next_number);
}

/*****************************************************************************/
/* table-lookup trig functions */

/* TRIGRES of 4096 gives an average error of .00049 & max error of .00153 in sin */
/* TRIGRES of 16384 gives an average error of .00012 & max error of .00038 in sin */
#define TRIGRES		4096	/* should be power of 2 */
#define TRIGMASK	4095	/* TRIGRES - 1 */
#define TRIGSCALE	651.89865f	/* TRIGRES / (2 PI) */

static float dpTSinTable[TRIGRES], dpTCosTable[TRIGRES], dpTTanTable[TRIGRES];

/* dpTTrigInit - initializes the tables for dpTSin, dpTCos, & dpTTan */
void dpTTrigInit(void)
{
 int i;
 double theta;
 for (i=0; i<TRIGRES; i++)
	{
	theta = i*M_PI*2.0/TRIGRES;
	dpTSinTable[i] = sin(theta);
	dpTCosTable[i] = cos(theta);
	dpTTanTable[i] = tan(theta);
	}
}

float dpTSin(float x)
{
 return dpTSinTable[((int)(x * TRIGSCALE) & TRIGMASK)];
}

float dpTCos(float x)
{
 return dpTCosTable[((int)(x * TRIGSCALE) & TRIGMASK)];
}

float dpTTan(float x)
{
 return dpTTanTable[((int)(x * TRIGSCALE) & TRIGMASK)];
}

/*****************************************************************************/

unsigned char *dpReadBinaryFile(char *fname,unsigned char *data,int size)
{
 if (!data) return NULL;
 if (!strcmp(&fname[strlen(fname)-2],".Z"))
	{
	FILE *pp;
	char com[256];
	sprintf(com,"zcat %s",fname);
	if ((pp=popen(com,"r")) == NULL)
		{
		perror(fname);
		return(NULL);
		}
	fread(data,size,1,pp);
	pclose(pp);
	}
 else {
	int fd;
	if ((fd = open(fname,O_RDONLY)) == -1)
		{
		perror(fname);
		return(NULL);
		}
	read(fd,data,size);
	close(fd);
	}
 return(data);
}

int dpWriteBinaryFile(char *fname,unsigned char *data,int size)
{
 int fd;
 if ((fd = creat(fname,0644)) == -1)
	{
	perror(fname);
	return(0);
	}
 write(fd,data,size);
 close(fd);
 return 1;
}

#include <sys/time.h>

static struct timeval start,end;

void dpBeginTiming(void)
{
 gettimeofday(&start,NULL);
}

float dpEndTiming(int print_time)
{
 float dt;
 gettimeofday(&end,NULL);
 dt = end.tv_sec-start.tv_sec + (float)(end.tv_usec-start.tv_usec)/1000000.0;
 if (print_time) printf("time = %f sec\n",dt);
 return(dt);
}
