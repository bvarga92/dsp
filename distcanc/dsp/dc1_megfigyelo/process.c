#include "process.h"

#pragma optimize_for_speed

complex x[N]; //ebben lesznek a megfigyelt egyutthatok
bool observerOn;

void process(){
	static float phi=0;
	float u=rightIn, y, e;
	complex c[N];
	unsigned i;
	/* ha ki van kapcsolva, nullazunk mindent */
	if(!observerOn){
		e=phi=0;
		for(i=0;i<N;i++) x[i].re=x[i].im=0;
		rightOut4=leftOut4=0;
		return;
	}
	/* c[0]=1, c[1]=exp(j*phi) */
	c[0].re=1;
	c[0].im=0;
	c[1].re=cos(phi);
	c[1].im=sin(phi);
	/* y=c*x, e=u-y */
	y=x[0].re+2*(c[1].re*x[1].re-c[1].im*x[1].im);
	for(i=2;i<N;i++){
		c[i].re=c[1].re*c[i-1].re-c[1].im*c[i-1].im;
		c[i].im=c[1].re*c[i-1].im+c[1].im*c[i-1].re;
		y+=2*(c[i].re*x[i].re-c[i].im*x[i].im);
	}
	e=u-y;
	/* x=x+ALPHA*e*conj(c) */
	for(i=0;i<N;i++){
		x[i].re+=ALPHA*e*c[i].re;
		x[i].im-=ALPHA*e*c[i].im;
	}
	/* diszkret ido frissitese */
	phi+=2*PI*F0/FS;
	if(phi>2*PI) phi-=2*PI;
	/* kimenetek */
	rightOut4=y;
	leftOut4=e;
}
