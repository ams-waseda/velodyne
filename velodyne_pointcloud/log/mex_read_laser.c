#include <stdio.h>
#include <stdint.h>
#include "mex.h"

#define N 10
#define M (1<<27)
#define MAXDATA (M*N)

typedef struct {
	uint32_t time;
	uint32_t cnt;
	uint32_t i;
	float x;
	float y;
	float z;
	float intensity;
	float ring;
	float azimuth;
	float firing;
} laser_t;

extern void mexFunction(int nargout, mxArray *argout[], int nargin,
					    const mxArray *argin[])
{
	unsigned long cnt=0,i;
	char file[256];
	double *data,*out;
	FILE *fp;
	laser_t laser;
	
	if ((data=(double *)malloc(MAXDATA*sizeof(double)))==NULL) {
		mexErrMsgTxt("malloc error");
	}
	
	if (mxGetString(argin[0],file,sizeof(file))!=0) {
		mexErrMsgTxt("argin error");
	}
	
	if ((fp=fopen(file, "rb")) == NULL) {
		mexErrMsgTxt("file open error");
	}
	
	while (fread(&laser,sizeof(laser_t),1,fp)>0) {
		data[N*cnt+0]=(double)laser.time;
		data[N*cnt+1]=(double)laser.cnt;
		data[N*cnt+2]=(double)laser.i;
		data[N*cnt+3]=(double)laser.x;
		data[N*cnt+4]=(double)laser.y;
		data[N*cnt+5]=(double)laser.z;
		data[N*cnt+6]=(double)laser.intensity;
		data[N*cnt+7]=(double)laser.ring;
		data[N*cnt+8]=(double)laser.azimuth;
		data[N*cnt+9]=(double)laser.firing;
		
		if (cnt==0) {
			mexPrintf("time:%d\n",laser.time);
		}
		
		cnt++;
		if (cnt==M) {
			mexErrMsgTxt("malloc tarinai");
			break;
		}
	}
	fclose(fp);
	
	argout[0]=mxCreateDoubleMatrix(N,cnt,mxREAL);
	out=mxGetPr(argout[0]);
	
	for (i=0;i<N*cnt;i++) out[i]=data[i];
	
	free(data); 
}
