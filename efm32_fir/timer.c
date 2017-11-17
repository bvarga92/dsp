#include "timer.h"
#include "em_timer.h"
#include "em_cmu.h"

volatile bool timerIntrp=false;

void TIMER0_IRQHandler(void){
	TIMER_IntClear(TIMER0,TIMER_IF_OF);
	timerIntrp=true;
}

void timerInit(uint32_t top){
	static TIMER_Init_TypeDef timerInitStruct;
	CMU_ClockEnable(cmuClock_TIMER0,true);
	timerInitStruct.enable=true;
	timerInitStruct.debugRun=true;
	timerInitStruct.prescale=timerPrescale1;
	timerInitStruct.clkSel=timerClkSelHFPerClk;
	timerInitStruct.count2x=false;
	timerInitStruct.ati=false;
	timerInitStruct.fallAction=timerInputActionNone;
	timerInitStruct.riseAction=timerInputActionNone;
	timerInitStruct.mode=timerModeUp;
	timerInitStruct.dmaClrAct=false;
	timerInitStruct.quadModeX4=false;
	timerInitStruct.oneShot=false;
	timerInitStruct.sync=false;
	TIMER_TopSet(TIMER0,top);
	TIMER_IntEnable(TIMER0,TIMER_IF_OF);
	NVIC_EnableIRQ(TIMER0_IRQn);
	TIMER_Init(TIMER0,&timerInitStruct);
}
