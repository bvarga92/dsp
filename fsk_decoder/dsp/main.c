/****************************************************************/
/*                                                              */
/* FSK dekoder                                                  */
/*                                                              */
/* A feher bemenetre adott FSK jelet dekodolja, a vett          */
/* karaktereket a string globalis tombbe menti.                 */
/*                                                              */
/* Parameterek:                                                 */
/*    - bit 0: 1 kHz                                            */
/*    - bit 1: 5 kHz                                            */
/*    - bitido: 1/48 masodperc                                  */
/*    - bitkozi szunet: fel bitido                              */
/*    - karakterkozi szunet: ketszeres bitido                   */
/*                                                              */
/****************************************************************/

#include "bsp/ad1835.h"
#include "bsp/clock.h"
#include "bsp/measureRuntime.h"
#include "bsp/buttonsLEDs.h"

extern void process(void);

void main(void){
	initPLL();
	initCodec();
	initRuntimeMeasure();
	initButtons();
	writeLEDs(0);
	while(1){
		if(dataReady){ //adat jott
			dataReady=0;
			isProcessing=1;
			runTimeSignalOn();
			process();
			runTimeSignalOff();
			isProcessing=0;
		}
	}
}
