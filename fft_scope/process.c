#include "bsp/ad1835.h"
#include <math.h>
#include <complex.h>
#include <filter.h>
#include <window.h>

#define NFFT 512 //FFT pontszama

float input[NFFT]; //bemenet mintai
float win_rect[NFFT], win_hanning[NFFT], win_blackman[NFFT]; //ablakfuggvenyek
float *window; //ablakfuggveny pointer
complex_float output[NFFT/2]; //itt all elo a spektrum
complex_float pm twiddle[NFFT/2]; //komplex egyutthatok tablaja

void initFFT(){
	unsigned i;
	twidfft(twiddle,NFFT); //twiddle table feltoltese
	for(i=0;i<NFFT;i++) win_rect[i]=1.0; //negyszogablak eloallitasa
	gen_hanning(win_hanning,1,NFFT); //Hanning-ablak eloallitasa
	gen_blackman(win_blackman,1,NFFT); //Blackman-ablak eloallitasa
	for(i=0;i<NFFT;i++){ //512-re normalas
		win_hanning[i]*=2.0039;
		win_blackman[i]*=2.3856;
	}
}

void setWindow(unsigned sel){ //ablakfuggveny beallitasa
	switch(sel){
		case 0:  window=win_rect; break;
		case 1:  window=win_hanning; break;
		default: window=win_blackman;
	}
}

void process(){
	static unsigned cntr=0;
	float temp[NFFT], out;
	unsigned i;
	input[cntr++]=rightIn; //toltjuk a buffert
	if(cntr==NFFT){ //megtelt a buffer, lehet FFT-zni
		cntr=0;
		for(i=0;i<NFFT;i++) input[i]*=window[i]; //ablakozas
		rfft(input,temp,output,twiddle,1,NFFT); //FFT
	}
	out=log10f(cabsf(output[cntr/2]))/5.0; //kimenet logaritmikus skalan
	rightOut4=(out>0)?fminf(out,1.0):fmaxf(out,-1.0); //csonkolas
	leftOut4=2*cntr/(float)NFFT-1; //fureszjel
}
