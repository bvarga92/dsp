#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI        3.141592653589793
#define FS        8000.0 //mintaveteli frekvencia
#define N_S       100    //a masodlagos utakat modellezo FIR szuro hossza
#define N_W       200    //az adaptiv szuro hossza
#define SAMP_ID   100000 //ennyi utemig tart az identifikacio
#define MU_IDENT  (1e-2) //batorsagi tenyezo az identifikaciohoz
#define MU_FXLMS  (1e-3) //batorsagi tenyezo a zajcsokkenteshez
#define TMAX      1000   //ennyi utemet szimulalunk

double S_id[N_S];        //az identifikalt masodlagos ut
double W[N_W];           //az FxLMS algoritmus adaptiv szuroje

/* tavoli zajforras */
double x(unsigned n){
    return sin(2*PI*440/FS*n);
}

/* elsodleges ut (zajforras --> mikrofon) */
double P(double in){
    static double in1, in2, in3, out1, out2, out3;
    double out=in2+0.12*in3-0.3*out1+0.5*out3;
    in3=in2; in2=in1; in1=in;
    out3=out2; out2=out1; out1=out;
    return out;
}

/* masodlagos ut (hangszoro --> mikrofon) */
double S(double in){
    static double in1, in2, out1, out2;
    double out=in-0.4164*in1+1.2346*in2-0.6627*out1-0.6414*out2;
    in2=in1; in1=in;
    out2=out1; out1=out;
    return out;
}

/* S identifikacioja */
double ident(double mic){
    static double y_buf[N_S];
    double y, e;
    static unsigned i=0;
    unsigned j, ix;
    /* a kimenet zaj */
    y=rand()/((double)RAND_MAX);
    /* e=mic-Y*S_id */
    y_buf[i]=y;
    ix=i;
    e=mic;
    for(j=0;j<N_S;j++,ix++) e-=y_buf[ix%N_S]*S_id[j];
    /* dS_id=mu*e*Y */
    for(j=0;j<N_S;j++) S_id[j]+=MU_IDENT*e*y_buf[(j+i)%N_S];
    i=i?(i-1):(N_S-1);
    return y;
}

/* zajcsokkentes */
double process(double x, double e){
    static double x_buf_W[N_W], x_buf_S[N_S], r_buf[N_W];
    static unsigned iW=0, iS=0;
    unsigned i, j;
    double y, r;
    /* y=-W*x */
    x_buf_W[iW]=x;
    i=iW;
    y=0;
    for(j=0;j<N_W;j++,i++) y-=x_buf_W[i%N_W]*W[j];
    /* r=S_id*x */
    x_buf_S[iS]=x;
    i=iS;
    r=0;
    for(j=0;j<N_S;j++,i++) r+=x_buf_S[i%N_S]*S_id[j];
    /* dW=mu*e*R */
    r_buf[iW]=r;
    for(j=0;j<N_W;j++) W[j]+=MU_FXLMS*e*r_buf[(j+iW)%N_W];
    /* frissites a kovetkezo utemre */
    iW=iW?(iW-1):(N_W-1);
    iS=iS?(iS-1):(N_S-1);
    return y;
}

/* szimulacio */
int main(){
    unsigned long i;
    double y, e, out=0;
    FILE *f;
    for(i=0;i<SAMP_ID;i++) out=ident(S(out));
    f=fopen("signals.dat","wt");
    for(i=0;i<N_S;i++) fprintf(f,"%f\t%f\n",S_id[i],0);
    for(i=0;i<TMAX;i++){
        y=P(x(i));
        e=y+S(out);
        out=process(x(i),e);
        fprintf(f,"%f\t%f\n",y,e);
    }
    fclose(f);
    f=fopen("signals_plot.m","wt");
    fprintf(f,"clear all;\n");
    fprintf(f,"clc;\n");
    fprintf(f,"data=load('signals.dat');\n");
    fprintf(f,"S_id=data(1:%u,1);\n",N_S);
    fprintf(f,"y=data(%u:end,1);\n",N_S+1);
    fprintf(f,"e=data(%u:end,2);\n",N_S+1);
    fprintf(f,"t=(0:(length(e)-1))/%f;\n",FS);
    fprintf(f,"f=(0:8191)*%f/8192;",FS);
    fprintf(f,"subplot(211);\n");
    fprintf(f,"plot(f,20*log10(abs(fft(S_id,8192))));\n");
    fprintf(f,"grid on;\n");
    fprintf(f,"xlim([0 %f]);\n",FS/2);
    fprintf(f,"xlabel('f');\n");
    fprintf(f,"ylabel('|S_{id}| [dB]');\n");
    fprintf(f,"subplot(212);\n");
    fprintf(f,"plot(t,y,t,e,'r');\n");
    fprintf(f,"xlabel('t');\n");
    fprintf(f,"xlim([0 max(t)]);\n");
    fprintf(f,"ylabel('y, e');\n");
    fclose(f);
    return 0;
}
