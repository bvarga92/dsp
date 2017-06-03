#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI           3.141592653589793
#define FS           8000.0 //mintaveteli frekvencia
#define N_S          5      //a masodlagos utakat modellezo FIR szurok hossza
#define N_W          200    //az adaptiv szurok hossza
#define MU           (1e-6) //batorsagi tenyezo
#define TMAX         3000   //ennyi utemet szimulalunk
#define COMM_PERIOD  30     //ennyi utemenkent kuldjuk az update-eket

/* a masodlagos utaknak megfelelo FIR szurok (a valosagban identifikacio eredmenye) */
double S11_id[N_S]={1,2,3,4,5};
double S12_id[N_S]={-10,9,-8,7,-6};
double S21_id[N_S]={-10,9,-8,7,-6};
double S22_id[N_S]={1,2,3,4,5};

/* a node-ok kozott megosztott valtozok (kommunikacio) */
int signal_from1to2=0;
int signal_from2to1=0;
double dW_from1to2[N_W];
double dW_from2to1[N_W];



/* tavoli zajforras */
double x(unsigned n){
    return sin(2*PI*200/FS*n);
}



/* elsodleges utak (zajforras --> mikrofon), most egyszeru IIR szurok */
double P1(double in){
    static double in1, in2, in3, out1, out2, out3;
    double out=in2+0.12*in3-0.3*out1+0.5*out3;
    in3=in2; in2=in1; in1=in;
    out3=out2; out2=out1; out1=out;
    return out;
}
double P2(double in){
    static double in1, in2, in3, out1, out2, out3;
    double out=in2+0.15*in3-0.2*out1+0.7*out3;
    in3=in2; in2=in1; in1=in;
    out3=out2; out2=out1; out1=out;
    return out;
}



/* masodlagos utak (hangszoro --> mikrofon), most a fenti Sxx_id szuroket hasznaljuk (tokeletes identifikacio) */
double S11(double in){
    static double buf[N_S];
    static unsigned i=0;
    unsigned j, ix;
    double out=0;
    buf[i]=in;
    ix=i;
    out=0;
    for(j=0;j<N_S;j++) out+=buf[ix++%N_S]*S11_id[j];
    i=(i==0)?(N_S-1):(i-1);
    return out;
}
double S12(double in){
    static double buf[N_S];
    static unsigned i=0;
    unsigned j, ix;
    double out=0;
    buf[i]=in;
    ix=i;
    out=0;
    for(j=0;j<N_S;j++) out+=buf[ix++%N_S]*S12_id[j];
    i=(i==0)?(N_S-1):(i-1);
    return out;
}
double S21(double in){
    static double buf[N_S];
    static unsigned i=0;
    unsigned j, ix;
    double out=0;
    buf[i]=in;
    ix=i;
    out=0;
    for(j=0;j<N_S;j++) out+=buf[ix++%N_S]*S21_id[j];
    i=(i==0)?(N_S-1):(i-1);
    return out;
}
double S22(double in){
    static double buf[N_S];
    static unsigned i=0;
    unsigned j, ix;
    double out=0;
    buf[i]=in;
    ix=i;
    out=0;
    for(j=0;j<N_S;j++) out+=buf[ix++%N_S]*S22_id[j];
    i=(i==0)?(N_S-1):(i-1);
    return out;
}



/* zajcsokkentes */
double node1(double x, double e){
    static double W[N_W], x_buf_W[N_W], x_buf_S[N_S], r11_buf[N_W], r21_buf[N_W], dW11[N_W], dW21[N_W];
    static unsigned iW=0, iS=0, commCntr=0;
    unsigned i, j;
    double y, r11, r21;
    /* y=-W*x */
    x_buf_W[iW]=x;
    i=iW;
    y=0;
    for(j=0;j<N_W;j++,i++) y-=x_buf_W[i%N_W]*W[j];
    /* r=S_id*x */
    x_buf_S[iS]=x;
    i=iS;
    r11=r21=0;
    for(j=0;j<N_S;j++,i++){
        r11+=x_buf_S[i%N_S]*S11_id[j];
        r21+=x_buf_S[i%N_S]*S21_id[j];
    }
    /* dW=mu*e*R */
    r11_buf[iW]=r11;
    r21_buf[iW]=r21;
    for(j=0;j<N_W;j++){
        dW11[j]+=MU*e*r11_buf[(j+iW)%N_W];
        dW21[j]+=MU*e*r21_buf[(j+iW)%N_W];
    }
    /* kommunikacio: dW21-et elkuldjuk node2-nek */
    if(commCntr==COMM_PERIOD-1){
        commCntr=0;
        for(j=0;j<N_W;j++){
            dW_from1to2[j]=dW21[j];
            dW21[j]=0;
        }
        signal_from1to2=1;
    }
    else{
        commCntr++;
    }
    /* egyutthatokeszlet frissitese, ha kaptunk update-et */
    if(signal_from2to1){
        for(j=0;j<N_W;j++){
            W[j]+=dW_from2to1[j]+dW11[j];
            dW11[j]=0;
        }
        signal_from2to1=0; //ack
    }
    /* frissites a kovetkezo utemre */
    iW=iW?(iW-1):(N_W-1);
    iS=iS?(iS-1):(N_S-1);
    return y;
}
double node2(double x, double e){
    static double W[N_W], x_buf_W[N_W], x_buf_S[N_S], r22_buf[N_W], r12_buf[N_W], dW22[N_W], dW12[N_W];
    static unsigned iW=0, iS=0, commCntr=0;
    unsigned i, j;
    double y, r22, r12;
    /* y=-W*x */
    x_buf_W[iW]=x;
    i=iW;
    y=0;
    for(j=0;j<N_W;j++,i++) y-=x_buf_W[i%N_W]*W[j];
    /* r=S_id*x */
    x_buf_S[iS]=x;
    i=iS;
    r22=r12=0;
    for(j=0;j<N_S;j++,i++){
        r22+=x_buf_S[i%N_S]*S22_id[j];
        r12+=x_buf_S[i%N_S]*S12_id[j];
    }
    /* dW=mu*e*R */
    r22_buf[iW]=r22;
    r12_buf[iW]=r12;
    for(j=0;j<N_W;j++){
        dW22[j]+=MU*e*r22_buf[(j+iW)%N_W];
        dW12[j]+=MU*e*r12_buf[(j+iW)%N_W];
    }
    /* kommunikacio: dW12-t elkuldjuk node1-nek */
    if(commCntr==COMM_PERIOD-1){
        commCntr=0;
        for(j=0;j<N_W;j++){
            dW_from2to1[j]=dW12[j];
            dW12[j]=0;
        }
        signal_from2to1=1;
    }
    else{
        commCntr++;
    }
    /* egyutthatokeszlet frissitese, ha kaptunk update-et */
    if(signal_from1to2){
        for(j=0;j<N_W;j++){
            W[j]+=dW_from1to2[j]+dW22[j];
            dW22[j]=0;
        }
        signal_from1to2=0; //ack
    }
    /* frissites a kovetkezo utemre */
    iW=iW?(iW-1):(N_W-1);
    iS=iS?(iS-1):(N_S-1);
    return y;
}



/* szimulacio */
int main(){
    unsigned long i;
    double y1, e1, y2, e2, out1=0, out2=0;
    FILE *f;
    f=fopen("signals.dat","wt");
    for(i=0;i<TMAX;i++){
        y1=P1(x(i));
        y2=P2(x(i));
        e1=y1+S11(out1)+S21(out2);
        e2=y2+S22(out2)+S12(out1);
        out1=node1(x(i),e1);
        out2=node2(x(i),e2);
        fprintf(f,"%f\t%f\t%f\t%f\n",y1,e1,y2,e2);
    }
    fclose(f);
    f=fopen("signals_plot.m","wt");
    fprintf(f,"clear all;\n");
    fprintf(f,"clc;\n");
    fprintf(f,"data=load('signals.dat');\n");
    fprintf(f,"y1=data(:,1);\n");
    fprintf(f,"e1=data(:,2);\n");
    fprintf(f,"y2=data(:,3);\n");
    fprintf(f,"e2=data(:,4);\n");
    fprintf(f,"t=(0:(size(data,1)-1))/%f;\n",FS);
    fprintf(f,"subplot(211);\n");
    fprintf(f,"plot(t,y1,t,e1,'r');\n");
    fprintf(f,"xlim([0 max(t)]);\n");
    fprintf(f,"subplot(212);\n");
    fprintf(f,"plot(t,y2,t,e2,'r');\n");
    fprintf(f,"xlim([0 max(t)]);\n");
    fclose(f);
    return 0;
}
