#include "process.h"
#include "iqmixer.h"
#include <signal.h>
#include <math.h>

static complex inv;

void ident(void){
	static float phi=0;
	int i;
	complex in_cpx, y, e;
	static complex c[2], x[2];
	// c=exp(1j*2*pi*(f_rf-f_lo)/fs*(ii-1))
	c[0].re=1;
	c[0].im=0;
	c[1].re=cos(phi);
	c[1].im=sin(phi);
	// in_cpx=in_i(ii)-1j*in_q(ii)
	in_cpx.re=in1;
	in_cpx.im=in2;
	// y=c*x
	y.re=y.im=0;
	for(i=0;i<2;i++){
		y.re+=c[i].re*x[i].re-c[i].im*x[i].im;
		y.im+=c[i].re*x[i].im+c[i].im*x[i].re;
	}
	// e=in_cpx-y;
	e.re=in_cpx.re-y.re;
	e.im=in_cpx.im-y.im;
	// x=x+alpha*e*c'
	for(i=0;i<2;i++){
		x[i].re+=ALPHA*(e.re*c[i].re+e.im*c[i].im);
		x[i].im+=ALPHA*(e.im*c[i].re-e.re*c[i].im);
	}
	// out_rf=0.8*(c[1].re*Q+c[1].re*I)
	out1=0.8*c[1].re;
	out2=0.8*c[1].im;
	// inv=1/x[1]
	inv.re=x[1].re/(x[1].re*x[1].re+x[1].im*x[1].im);
	inv.im=-x[1].im/(x[1].re*x[1].re+x[1].im*x[1].im);
	// frissites a kovetkezo utemre
	phi+=2*PI*(FIN-FLO)/FS;
	if(phi>2*PI) phi-=2*PI;
}
	
void process(void){
	static float phi=0;
	int i;
	complex in_cpx, y, e, r;
	static complex c[2], x[2], p;
	// c=exp(1j*2*pi*(f_rf-f_lo)/fs*(ii-1))
	c[0].re=1;
	c[0].im=0;
	c[1].re=cos(phi);
	c[1].im=sin(phi);
	// in_cpx=in_i(ii)-1j*in_q(ii)
	in_cpx.re=in1;
	in_cpx.im=in2;
	// y=c*x
	y.re=y.im=0;
	for(i=0;i<2;i++){
		y.re+=c[i].re*x[i].re-c[i].im*x[i].im;
		y.im+=c[i].re*x[i].im+c[i].im*x[i].re;
	}
	// r=c[1]*p
	r.re=c[1].re*p.re-c[1].im*p.im;
	r.im=c[1].re*p.im+c[1].im*p.re;
	// e=in_cpx-y
	e.re=in_cpx.re-y.re;
	e.im=in_cpx.im-y.im;
	// x=x+alpha*e*c'
	for(i=0;i<2;i++){
		x[i].re+=ALPHA*(e.re*c[i].re+e.im*c[i].im);
		x[i].im+=ALPHA*(e.im*c[i].re-e.re*c[i].im);
	}
	// p=p+beta.*x
	p.re+=BETA*(inv.re*x[1].re-inv.im*x[1].im);
	p.im+=BETA*(inv.re*x[1].im+inv.im*x[1].re);
	// rf_out(ii)=real(r)*lo_i(ii)-imag(r)*lo_q(ii)
	out1=-r.re;
	out2=-r.im;
	// frissites a kovetkezo utemre
	phi+=2*PI*(FIN-FLO)/FS;
	if(phi>2*PI) phi-=2*PI;
}
