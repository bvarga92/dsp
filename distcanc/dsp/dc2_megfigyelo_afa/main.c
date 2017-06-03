/*******************************************************/
/*                                                     */
/* Rezonatoros megfigyelo + AFA      Varga Balazs 2016 */
/*                                                     */
/*     Bemenet: feher                                  */
/*     Kimenet: CH4 feher (jack bal)                   */
/*     Hibajel: CH4 piros (jack jobb)                  */
/*                                                     */
/*     SW1 gomb be-, SW2 pedig kikapcsolja (nullarol   */
/*     inditja) a megfigyelot.                         */
/*                                                     */
/*******************************************************/

#include "bsp/ad1835.h"
#include "bsp/clock.h"
#include "bsp/buttonsLEDs.h"
#include "process.h"

void main(void){
	initPLL();
	initCodec();
	initButtons();
	writeLEDs(0);
	observerOn=false;
	while(1){
		if(dataReady){ //adat jott
			dataReady=0;
			isProcessing=1;
			process();
			isProcessing=0;
		}
		else if(readSW1()){ //megfigyelo be
			writeLEDs(255);
			observerOn=true;
		}
		else if(readSW2()){ //megfigyelo ki
			writeLEDs(0);
			observerOn=false;
		}
	}
}
