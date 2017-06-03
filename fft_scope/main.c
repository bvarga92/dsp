/****************************************************************/
/*                                                              */
/* Spektrumanalizator                                           */
/*                                                              */
/* A bemenet a feher csatorna, a 4-es kimeneten megjelenik a    */
/* jel spektruma, valamint egy fureszjel az oszcilloszkop X-Y   */
/* uzemmodjahoz (vagy triggereleshez). Az SW1-SW3 gombokkal     */
/* valaszthatunk ablakfuggvenyt.                                */
/* 512 pontos FFT, 48 kHz mintavetel --> 93.75 Hz felbontas.    */
/*                                                              */
/****************************************************************/

#include "bsp/ad1835.h"
#include "bsp/clock.h"
#include "bsp/measureRuntime.h"
#include "bsp/buttonsLEDs.h"

extern void initFFT(void);
extern void setWindow(unsigned sel);
extern void process(void);

void main(void){
	initPLL();
	initCodec();
	initRuntimeMeasure();
	initButtons();
	initFFT();
	writeLEDs(4);
	setWindow(2);
	while(1){
		if(dataReady){ //adat jott
			dataReady=0;
			isProcessing=1;
			runTimeSignalOn();
			process();
			runTimeSignalOff();
			isProcessing=0;
		}
		if(readSW1()){ //rect ablak
			writeLEDs(1);
			setWindow(0);
		}
		else if(readSW2()){ //Hanning-ablak
			writeLEDs(2);
			setWindow(1);
		}
		else if(readSW3()){ //Blackman-ablak
			writeLEDs(4);
			setWindow(2);
		}
	}
}
