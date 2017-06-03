#include "bsp/ad1835.h"
#include "process.h"
#include <math.h>

complex beta[N];

void ident(unsigned i, unsigned samples, complex* transfer){
	static float phi=0;
	float y, e;
	static complex x[N];
	complex c[N];
	unsigned k;
	for(;samples;samples--){
		while(!dataReady) ; //megvarjuk, amig jon minta
		dataReady=0;
		isProcessing=1;
		c[0].re=1;
		c[0].im=0;
		c[1].re=cos(phi);
		c[1].im=sin(phi);
		y=x[0].re+2*(c[1].re*x[1].re-c[1].im*x[1].im);
		for(k=2;k<N;k++){
			c[k].re=c[1].re*c[k-1].re-c[1].im*c[k-1].im;
			c[k].im=c[1].re*c[k-1].im+c[1].im*c[k-1].re;
			y+=2*(c[k].re*x[k].re-c[k].im*x[k].im);
		}
		e=rightIn-y;
		for(k=0;k<N;k++){
			x[k].re+=ALPHA*e*c[k].re;
			x[k].im-=ALPHA*e*c[k].im;
		}
		leftOut4=y;
		rightOut4=0.5*(c[i].re); //ez a szinuszos merojel
		phi+=2*PI*F0/FS;
		if(phi>2*PI) phi-=2*PI;
		isProcessing=0;
	}
	transfer->re=x[i].re;
	transfer->im=x[i].im;
}

#pragma optimize_for_speed
void process(){
	static float phi=0;
	float u=rightIn, y, e, r;
	static complex x[N], p[N];
	complex c[N];
	unsigned i;
	/* c[0]=1, c[1]=exp(j*phi) */
	c[0].re=1;
	c[0].im=0;
	c[1].re=cos(phi);
	c[1].im=sin(phi);
	/* y=c*x, r=c*p, e=u-y */
	y=x[0].re+2*(c[1].re*x[1].re-c[1].im*x[1].im);
	r=p[0].re+2*(c[1].re*p[1].re-c[1].im*p[1].im);
	for(i=2;i<N;i++){
		c[i].re=c[1].re*c[i-1].re-c[1].im*c[i-1].im;
		c[i].im=c[1].re*c[i-1].im+c[1].im*c[i-1].re;
		y+=2*(c[i].re*x[i].re-c[i].im*x[i].im);
		r+=2*(c[i].re*p[i].re-c[i].im*p[i].im);
	}
	e=u-y;
	/* x=x+ALPHA*e*conj(c), p=p+beta*x */
	for(i=0;i<N;i++){
		p[i].re+=beta[i].re*x[i].re-beta[i].im*x[i].im;
		p[i].im+=beta[i].re*x[i].im+beta[i].im*x[i].re;
		x[i].re+=ALPHA*e*c[i].re;
		x[i].im-=ALPHA*e*c[i].im;
	}
	/* az alapharmonikust nem szabalyozzuk le */
	p[1].re=p[1].im=0;
	/* diszkret ido frissitese */
	phi+=2*PI*F0/FS;
	if(phi>2*PI) phi-=2*PI;
	/* kimenet */
	rightOut4=-r;	
}
