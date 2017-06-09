/****************************************************/
/*                                                  */
/* A nyomogombokkal valaszthatunk effektet:         */
/*     SW1: nincs effekt                            */
/*     SW2: echo                                    */
/*     SW3: slapback                                */
/*                                                  */
/* Az atkapcsolasi tranziensek nincsenek kezelve!   */
/*                                                  */
/****************************************************/

#include "bsp/ad1835.h"
#include "bsp/clock.h"
#include "bsp/measureRuntime.h"
#include "bsp/buttonsLEDs.h"

extern char currentEffect;
extern void process(void);

void main(void){
	currentEffect=1;
	initPLL();
	initSPORT();
	initDAI();
	initRuntimeMeasure();
	initCodec();
	interrupt(SIG_SP0,receive);
	initButtons();
	while(1){
		if(dataReady){
			dataReady=0;
			isProcessing=1;
			runTimeSignalOn(); //futasido merheto a DAIP15 labon
			process(); //itt tortenik a feldolgozas
			runTimeSignalOff();
			isProcessing=0;
		}
		if(readSW1()) currentEffect=0;
		else if(readSW2()) currentEffect=1;
		else if(readSW3()) currentEffect=2;
		writeLEDs(1<<currentEffect);
	}
}
