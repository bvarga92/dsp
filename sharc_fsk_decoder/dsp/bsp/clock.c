#include "clock.h"
#include <cdef21364.h>
#include <def21364.h>

/* orajel beallitasa: 24.576*27/2=331.776 MHz */
void initPLL(){
	unsigned int i, temp;
	temp=*pPMCTL;
	temp&=~(0xff);
	temp|=PLLM27|INDIV|DIVEN;
	*pPMCTL=temp;
	temp|=PLLBP;
	*pPMCTL=temp;
	for(i=0;i<4096;i++) asm("nop;");
	temp=*pPMCTL;
	temp^=PLLBP;
	*pPMCTL=temp;
}
