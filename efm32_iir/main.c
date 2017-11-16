/*********************************************************/
/*                                                       */
/*  IIR szuro (exponencialis atlagolo)                   */
/*                                                       */
/*    - bemenet: ADC0/CH5 (PD5, exp. header 14)          */
/*    - kimenet: DAC0/CH0 (PB11, exp. header 11)         */
/*                                                       */
/*********************************************************/

#include "board.h"
#include "lcd.h"
#include "timer.h"
#include "dac.h"
#include "adc.h"

#define ALPHA 0.9f

void process(uint16_t in, float *out){
	static float y=0;
	y=y+(1-ALPHA)*(in-y);
	*out=y;
}

int main(void){
	float in, out=0;
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
			dacWrite((uint16_t)out);
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
