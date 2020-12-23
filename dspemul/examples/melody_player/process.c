#include "fr1x16.h"

#define N 40 //hangok (es szunetek) szama

extern fract16 inLeft, inRight, outLeft, outRight; //a be- es kimeno sztereo jelparok

/* a hangok frekvenciai, amplitudoi es idotartamai --> program inditasa: main.exe 576000 */
const fract16 freq[N] = {  450,   425,   450,   425,   450,   337,   401,   357,   300,     0,   179,   225,   300,   337,     0,   225,   284,   337,   357,     0,   225,   450,   425,   450,   425,   450,   337,   401,   357,   300,     0,   179,   225,   300,   337,     0,   225,   357,   337,   300};
const fract16 ampl[N] = {32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767,     0, 32767, 32767, 32767, 32767,     0, 32767, 32767, 32767, 32767,     0, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767,     0, 32767, 32767, 32767, 32767,     0, 32767, 32767, 32767, 32767};
const int duration[N] = {12000, 12000, 12000, 12000, 12000, 12000, 12000, 12000, 23957, 12043, 12000, 12000, 12000, 24035, 11965, 12000, 12000, 12000, 23973, 12027, 12000, 12000, 12000, 12000, 12000, 12000, 12000, 12000, 12000, 24023, 11977, 12000, 12000, 12000, 24035, 11965, 12000, 12000, 12000, 48000};

void init(){}

void process(){
    static fract16 phi; //a generalt szinuszjel fazisa
    static int sampleCntr = 0, noteCntr = 0; //szamlalo az idotartamhoz es az eppen jatszott hanghoz
    phi = (phi + freq[noteCntr]) & 0x7FFF; //szinuszjel fazisanak frissitese az aktualis frekvencia alapjan
    if(ampl[noteCntr] == 0) phi = 0; //hogy a szunetek utan nulla kezdofazissal induljon a jel --> folytonos, nincs pattogas
    outLeft = outRight = mult_fr1x16(ampl[noteCntr], sin2pi_fr16(phi)); //kiadjuk a szinuszjelet a megfelelo amplitudoval
    /* szamlalok frissitese a kovetkezo utemre */
    sampleCntr++;
    if(sampleCntr == duration[noteCntr]){
        noteCntr = (noteCntr + 1) % N;
        sampleCntr = 0;
    }
}
