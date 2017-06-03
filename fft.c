#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.141592653589793
#define NFFT 256 //FFT pontszama (kettohatvany!)
#define WINDOWING 1 //ablakozzunk-e vagy sem

typedef struct{double re,im;} cpx; //komplex szam tipus

/* inicializalja a bazisegyutthatok tablajat */
void fftInit(cpx* twiddle, unsigned N){
    unsigned i;
    for(i=0;i<N/2;i++){
        twiddle[i].re=cos(2*PI/((double)N)*i);
        twiddle[i].im=-sin(2*PI/((double)N)*i);
    }
}

/* N pontos nonrekurziv FFT */
void fft(double* input, cpx* output, cpx* twiddle, unsigned N){
    unsigned bits, i, irev, j, i1, i2, itw;
    cpx temp1, temp2;
    bits=(unsigned)log2(N);
    /* bitreverse cimzes */
    for(i=0;i<N;i++){
        irev=i;
        for(j=0;j<bits-1;j++) irev=((irev<<1)&(N-1))|((i>>(j+1))&1);
        output[irev].re=input[i];
        output[irev].im=0;
    }
    /* nonrekurziv FFT */
    for(i=0;i<bits;i++) //hanyadik FFT szint
        for(j=0;j<N/2;j++){ //szinten belul hanyadik butterfly muvelet
            i1=((j<<(i+1))|(j>>(bits-i-1)))&(N-1); //egyik bemenet indexe
            i2=((j<<(i+1))|(1<<i)|(j>>(bits-i-1)))&(N-1); //masik bemenet indexe
            itw=(j>>(bits-i-1))<<(bits-i-1); //egyutthato indexe
            temp1.re=output[i1].re+twiddle[itw].re*output[i2].re-twiddle[itw].im*output[i2].im;
            temp1.im=output[i1].im+twiddle[itw].re*output[i2].im+twiddle[itw].im*output[i2].re;
            temp2.re=output[i1].re-twiddle[itw].re*output[i2].re+twiddle[itw].im*output[i2].im;
            temp2.im=output[i1].im-twiddle[itw].re*output[i2].im-twiddle[itw].im*output[i2].re;
            output[i1]=temp1;
            output[i2]=temp2;
        }
}

/* a window tombot feltolti egy N-re normalt Blackman-ablak mintaival */
void blackman(double* window, unsigned N){
    unsigned i;
    double sum=0;
    for(i=0;i<N;i++){
        window[i]=0.4266-0.4966*cos(2*PI*i/((double)(N-1)))+0.0768*cos(4*PI*i/((double)(N-1)));
        sum+=window[i];
    }
    for(i=0;i<N;i++) window[i]=window[i]*N/sum;
}

/* visszaadja a kompex szam abszoluterteket */
double cabs(cpx c){
    return sqrt(c.re*c.re+c.im*c.im);
}

/* visszaadja az atadott szam erteket decibelben */
double decibel(double f){
    return (f==0.0)?(-100.0):(20*log10(f));
}

int main(){
    unsigned i;
    double x[NFFT]; //bemenet
    double window[NFFT]; //ablakfuggveny
    cpx twiddle[NFFT/2]; //bazisegyutthatok
    cpx y[NFFT]; //ebben lesz a bemenet FFT-je
    for(i=0;i<NFFT;i++) x[i]=sin(2*PI/(double)NFFT*29.5*i); //nem koherensen mintavetelezett szinuszjel
    fftInit(twiddle,NFFT); //twiddle tabla feltoltese
    blackman(window,NFFT); //ablakfuggveny eloallitasa
    #if WINDOWING
        for(i=0;i<NFFT;i++) x[i]*=window[i]; //ablakozas
    #endif
    fft(x,y,twiddle,NFFT); //FFT
    for(i=0;i<NFFT;i++){y[i].re*=1.0/NFFT; y[i].im*=1.0/NFFT;} //normalas
    for(i=0;i<NFFT;i++) printf("%4u: %8.4f %+8.4f*j  -->  %5.1f dB\n",i,y[i].re,y[i].im,decibel(cabs(y[i])));
    return 0;
}
