#include "bsp/ad1835.h"

#define N 1199

#pragma align 4
float pm coeffsL[N]={
	#include "coeffsL.txt"
};

#pragma align 4
float pm coeffsR[N]={
	#include "coeffsR.txt"
};

#pragma align 4
float dm inputL[N];

#pragma align 4
float dm inputR[N];

#pragma optimize_for_speed
void process(){
	static int i=0;
	int j, ix;
	float outL, outR;
	inputL[i]=leftIn;
	inputR[i]=rightIn;
	ix=i;
	outL=0;
	for(j=0;j<N;j++) outL+=inputL[ix++%N]*coeffsL[j];
	ix=i;
	outR=0;
	for(j=0;j<N;j++) outR+=inputR[ix++%N]*coeffsR[j];
	i=(i==0)?(N-1):(i-1);
	leftOut4=outL;
	rightOut4=outR;
}
