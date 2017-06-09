/****************************************************************/
/*                                                              */
/* Sztereo FIR szuro                                            */
/*                                                              */
/* A kimenet a 4-es csatorna (vagy a fejhallgato jack). A szuro */
/* egyutthatoi a mellekelt MATLAB szkripttel generalhatok. A    */
/* peldaban a bal csatornan egy alulatereszto, a jobbon pedig   */
/* egy felulatereszto mukodik, mindketto 4 kHz-nel vag.         */
/*                                                              */
/* Projekt beallitasok:                                         */
/*   Project --> Project Options --> Compile/General            */
/*       Enable optimization                                    */
/*       Additional options: -force-circbuf                     */
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
		if(dataReady){
			dataReady=0;
			isProcessing=1;
			runTimeSignalOn();
			process();
			runTimeSignalOff();
			isProcessing=0;
		}
	}
}
