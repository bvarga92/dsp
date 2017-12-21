#include <cdef21364.h>
#include <def21364.h>
#include <SRU21364.h>
#include "clock.h"
#include "iqmixer.h"
#include "buttonsLEDs.h"
#include "process.h"

void main(void){
	unsigned identCntr=0, state=0;
	initPLL();
	iqInitDDS();
	iqInitCodec();
	iqSetDDSFreq(FLO);
	writeLEDs(0);
	while(1){
		if(dataReady){
			dataReady=0;
			if((state==0 || state==1)&&readSW1()){ //amig nyomjuk az SW1 gombot, addig identifikalunk
				state=1;
				ident();
			}
			else if((state==1)&&(!readSW1())){
				state=2;
				writeLEDs(255);
			}
			if(state==2){ //SW1 felengedese utan indul a szabalyzo
				process();
			}
		}
	}
}
