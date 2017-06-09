#include "measureRuntime.h"

/* a DAIP15 labat beallitja kimenetkent */
void initRuntimeMeasure(){
	SRU(HIGH,PBEN15_I);
	SRU(LOW,DAI_PB15_I);
}

/* bekapcsolja a DAIP15 labat */
void runTimeSignalOn(){
	SRU(HIGH,DAI_PB15_I);	
}

/* kikapcsolja a DAIP15 labat */
void runTimeSignalOff(){
	SRU(LOW,DAI_PB15_I);
}
