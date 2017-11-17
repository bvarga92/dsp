/*********************************************************/
/*                                                       */
/*  FIR szuro                                            */
/*                                                       */
/*    - fixpontos aritmetika (s.15)                      */
/*    - dupla buffer, 4-es MAC ciklus unroll             */
/*    - bemenet: ADC0/CH5 (PD5, exp. header 14)          */
/*    - kimenet: DAC0/CH0 (PB11, exp. header 11)         */
/*                                                       */
/*********************************************************/

#include "board.h"
#include "lcd.h"
#include "timer.h"
#include "dac.h"
#include "adc.h"

#define N 64

int32_t coeffs[N]={
	#include "../src/coeffs.dat"
};

void process(uint16_t in, uint16_t *out){
	static int32_t input[2*N+4];
	static uint16_t i=0;
	uint16_t j;
	int32_t acc=0;
	i=(i==0)?(N-1):(i-1);
	input[i]=input[i+N]=in;
	if(i<4) input[2*N+i]=in;
	for(j=0;j<N;j+=4) acc+=coeffs[j]*input[i+j]+coeffs[j+1]*input[i+j+1]+coeffs[j+2]*input[i+j+2]+coeffs[j+3]*input[i+j+3];
	acc>>=15;
	if(acc<0)
		*out=0;
	else if(acc>4095)
		*out=4095;
	else
		*out=acc;
}

int main(void){
	uint16_t in, out=0;
	bool filterOn=false;
	boardInit();
	lcdInit(false);
	lcdAlphaPrint("LPF OFF");
	timerInit(3000); //8 kHz mintaveteli frekvencia
	dacInit();
	adcInit();
	adcStart();
	while(1){
		if(timerIntrp){
			timerIntrp=false;
			ledOn(0);
			dacWrite(out);
			in=adcRead();
			adcStart();
			if(filterOn)
				process(in,&out);
			else
				out=in;
			ledOff(0);
		}
		if(pbGet(0) && !filterOn){
			filterOn=true;
			lcdAlphaPrint("LPF ON");
		}
		if(pbGet(1) && filterOn){
			filterOn=false;
			lcdAlphaPrint("LPF OFF");
		}
	}
}
