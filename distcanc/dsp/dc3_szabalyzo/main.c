/*******************************************************************************************/
/*                                                                                         */
/* Aktiv torzitascsokkentes                                              Varga Balazs 2016 */
/*                                                                                         */
/* Eloszor identifikaljuk az erosito atvitet. A CH4 feher kimeneten kiadunk egymas utan    */
/* egy-egy szinuszos jelet a felharmonikusok frekvenciain. A rendszer valaszat a DSP a     */
/* feher bemeneten meri vissza. A rendszer megfigyelt valasza megjelenik a CH4 piros       */
/* kimeneten. Identifikacio utan atallunk szabalyozasi uzemmodra.                          */
/*                                                                                         */
/* Bekotes:                                                                                */
/*   CH4 feher kimenet --- erosito kompenzalo (kivono) bemenete                            */
/*   feher bemenet     --- erosito kimenete                                                */
/*                                                                                         */
/*******************************************************************************************/

#include "bsp/ad1835.h"
#include "bsp/clock.h"
#include "bsp/buttonsLEDs.h"
#include "process.h"

void main(void){
	unsigned i;
	complex h;
	initPLL();
	initCodec();
	initButtons();
	/* identifikacio */
	beta[0].re=beta[0].im=0;
	for(i=1;i<N;i++){
		writeLEDs(i);
		ident(i,10000,&h);
		beta[i].re= h.re/(h.re*h.re+h.im*h.im)*BETA;
		beta[i].im=-h.im/(h.re*h.re+h.im*h.im)*BETA;
	}
	writeLEDs(255);
	/* identifikacio kesz */
	while(1){
		if(dataReady){ //adat jott
			dataReady=0;
			isProcessing=1;
			process();
			isProcessing=0;
		}
	}
}
