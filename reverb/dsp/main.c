/****************************************************************/
/*                                                              */
/* Sztereo reverb effekt                      Varga Balazs 2015 */
/*                                                              */
/* Az SW1 nyomogomb csokkenti, az SW2 pedig noveli az effekt    */
/* erosseget (a hozzakeveresi arany valtozik 2 szazalekonkent), */
/* ezt az 1-7 szamu LED-ek is visszajelzik. A kimenet a 4-es    */
/* csatorna.                                                    */
/*                                                              */
/* Presetek:                                                    */
/*    SW3 - nagy terem (LED8 sotet)                             */
/*    SW4 - kicsi szoba (LED8 vilagit)                          */
/*                                                              */
/* Projekt beallitasok:                                         */
/*    Project --> Project Options --> Compile/General           */
/*        Enable optimization                                   */
/*        Additional options: -force-circbuf                    */
/*                                                              */
/****************************************************************/

#include "bsp/ad1835.h"
#include "bsp/clock.h"
#include "bsp/measureRuntime.h"
#include "bsp/buttonsLEDs.h"

extern void process(void);
extern unsigned char wet, currentEffect;

typedef enum{IDLE, INC_BTN_PRESSED, DEC_BTN_PRESSED, BH_BTN_PRESSED, SR_BTN_PRESSED} stateType;

void main(void){
	stateType state=IDLE;
	initPLL();
	initCodec();
	initRuntimeMeasure();
	initButtons();
	wet=12;
	currentEffect=0;
	while(1){
		if(dataReady){ //jott adat a kodektol, fel kell dolgozni
			dataReady=0;
			isProcessing=1;
			runTimeSignalOn(); //futasido merheto a DAIP15 labon
			process(); //itt tortenik a feldolgozas, lasd process.c
			runTimeSignalOff();
			isProcessing=0;
		}
		else if(readSW1() && (state==IDLE)){ //megnyomtak a - gombot
			state=DEC_BTN_PRESSED;
			if(wet>=2) wet-=2;
		}
		else if(!readSW1() && (state==DEC_BTN_PRESSED)){ //felengedtek a - gombot
			state=IDLE;
		}
		else if(readSW2() && (state==IDLE)){ //megnyomtak a + gombot
			state=INC_BTN_PRESSED;
			if(wet<=98) wet+=2;
		}
		else if(!readSW2() && (state==INC_BTN_PRESSED)){ //felengedtek a + gombot
			state=IDLE;
		}
		else if(readSW3() && (state==IDLE)){ //megnyomtak a nagy termet kivalaszto gombot
			state=BH_BTN_PRESSED;
			currentEffect=0;
		}
		else if(!readSW3() && (state==BH_BTN_PRESSED)){ //felengedtek a nagy termet kivalaszto gombot
			state=IDLE;
		}
		else if(readSW4() && (state==IDLE)){ //megnyomtak a kis szobat kivalaszto gombot
			state=SR_BTN_PRESSED;
			currentEffect=1;
		}
		else if(!readSW4() && (state==SR_BTN_PRESSED)){ //felengedtek a kis szobat kivalaszto gombot
			state=IDLE;
		}
		else{
			writeLEDs(wet|(currentEffect<<7));
		}
	}
}
