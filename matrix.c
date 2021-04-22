#include <stdio.h>
#include <gl/gl.h>
#include "dave.h"


void dpPrintMatrix(Matrix m)
{
 int j;
 for (j=0; j<4; j++)
	printf("[ %f %f %f %f ]\n",m[0][j],m[1][j],m[2][j],m[3][j]);
}

int dpInvertAffineMatrix(Matrix in,Matrix out)
{
 double det_l,pos=0,neg=0,temp;
#define ACCUMULATE \
	if (temp >= 0.0) pos += temp; \
	else neg += temp;
 temp = in[0][0] * in[1][1] * in[2][2];
 ACCUMULATE
 temp = in[0][1] * in[1][2] * in[2][0];
 ACCUMULATE
 temp = in[0][2] * in[1][0] * in[2][1];
 ACCUMULATE
 temp = -in[0][2] * in[1][1] * in[2][0];
 ACCUMULATE
 temp = -in[0][1] * in[1][0] * in[2][2];
 ACCUMULATE
 temp = -in[0][0] * in[1][2] * in[2][1];
 ACCUMULATE
 det_l = (pos + neg);
 if (fabs(det_l/(pos-neg)) < 1e-15)
	{
	fprintf(stderr,"ERROR: dpInvertAffineMatrix: matrix is not invertable\n");
	return 0;
	}
 det_l = 1.0/det_l;
 out[0][0] = (in[1][1]*in[2][2] - in[1][2]*in[2][1]) * det_l;
 out[1][0] = (in[1][2]*in[2][0] - in[1][0]*in[2][2]) * det_l;
 out[2][0] = (in[1][0]*in[2][1] - in[1][1]*in[2][0]) * det_l;
 out[0][1] = (in[0][2]*in[2][1] - in[0][1]*in[2][2]) * det_l;
 out[1][1] = (in[0][0]*in[2][2] - in[0][2]*in[2][0]) * det_l;
 out[2][1] = (in[0][1]*in[2][0] - in[0][0]*in[2][1]) * det_l;
 out[0][2] = (in[0][1]*in[1][2] - in[0][2]*in[1][1]) * det_l;
 out[1][2] = (in[0][2]*in[1][0] - in[0][0]*in[1][2]) * det_l;
 out[2][2] = (in[0][0]*in[1][1] - in[0][1]*in[1][0]) * det_l;
 out[3][0] = -(in[3][0]*out[0][0] + in[3][1]*out[1][0] + in[3][2]*out[2][0]);
 out[3][1] = -(in[3][0]*out[0][1] + in[3][1]*out[1][1] + in[3][2]*out[2][1]);
 out[3][2] = -(in[3][0]*out[0][2] + in[3][1]*out[1][2] + in[3][2]*out[2][2]);
 out[0][3] = out[1][3] = out[2][3] = 0.0;
 out[3][3] = 1.0;
 return 1;
}

void dpMatrixMultiply(Matrix a,Matrix b,Matrix out)
{
 int i,j,k;
 Matrix result;
 float sum;
 for (i=0; i<4; i++)
    for (j=0; j<4; j++)
	{
	sum = 0;
	for (k=0; k<4; k++)
		sum += b[i][k] * a[k][j];
	result[i][j] = sum;
	}
 for (i=0; i<4; i++)
    for (j=0; j<4; j++)
	out[i][j] = result[i][j];
}
