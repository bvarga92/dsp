#include "bsp/ad1835.h"
#include "bsp/buttonsLEDs.h"

#define SAMPLES 100 //ablak merete
#define STRLEN 50 //veteli buffer hossza

float input[SAMPLES]; //bemenet mintai
char string[STRLEN]; //ide veszunk

void decode(void){
	unsigned i, n, level, detect;
	static unsigned cntr, detect_prev, bitcntr, charcntr;
	static char ch;
	bool sign, sign_prev;
	/* nullatmenetek szamolasa es jelszint merese */
	n=level=0;
	sign_prev=false;
	for(i=0;i<SAMPLES;i++){
		sign=(input[i]>0);
		if((i>0)&&(sign!=sign_prev)) n++;
		sign_prev=sign;
		if((input[i]>0.1)||(input[i]<-0.1)) level++;
	}
	/* dontes */
	detect=0;
	if(level>20){ //van jel
		if(n>17) //es 5 kHz-es
			detect=5;
		else if(n>2) //es 1 kHz-es
			detect=1;
	}
	/* szamlalas */
	cntr=(detect==detect_prev)?(cntr+1):0;
	detect_prev=detect;
	/* allapotgep */
	if((cntr>15)&&(detect==0)){ //karakter vege
		cntr=0;
		bitcntr=0;
		if(ch){
			string[charcntr]=ch;
			charcntr=(charcntr+1)%STRLEN;
			writeLEDs(ch);
			ch=0;
		}
	}
	else if((cntr>6)&&(detect==1)){ //0 bit
		cntr=0;
		if(bitcntr<8) bitcntr++;
	}
	else if((cntr>6)&&(detect==5)){ //1 bit
		cntr=0;
		if(bitcntr<8) ch|=1<<(bitcntr++);
	}
}

void process(){
	static unsigned cntr=0;
	input[cntr++]=rightIn; //toltjuk a buffert
	if(cntr==SAMPLES){ //megtelt a buffer, lehet szamolni
		cntr=0;
		decode();
	}
	rightOut4=rightIn;
	leftOut4=leftIn;
}
