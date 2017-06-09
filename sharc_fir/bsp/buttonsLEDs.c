#include "buttonsLEDs.h"

/* DAIP19 es 20 bemenet */
void initButtons(){
	SRU(LOW,PBEN19_I);
	SRU(LOW,PBEN20_I);
}

/* FLAG kiolvasasa */
int flagTest(int mask){
	int tmpANC0;
	asm volatile(
	        " %0 = flags;\n\t"
	        : "=&d" (tmpANC0)
	        :
	        :
	        );
	return tmpANC0&mask;
}

/* DAI bemenet kiolvasasa */
int readDAIpin(int mask){
	int tmpR;
	asm volatile(
	     		"%0 = flags;\n\t"
	     		: "=&F" (tmpR)
	     		:
	     		:
	     		);
	tmpR&=FLG14;
	return (*pDAI_PIN_STAT)&mask;
}

/* megjeleniti a LED-eken az atadott erteket (0x400000 cimre iras) */
void writeLEDs(int ledValue){
	*pPPCTL=0;
	*pIIPP=(int)&ledValue;
	*pIMPP=1;
	*pICPP=1;
	*pEMPP=1;
	*pECPP=1;
	*pEIPP=0x400000;
	*pPPCTL=PPTRAN|PPBHC|PPDUR20|PPDEN|PPEN;
}
