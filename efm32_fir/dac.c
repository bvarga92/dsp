#include "dac.h"
#include "em_cmu.h"
#include "em_dac.h"

void dacInit(void){
	DAC_Init_TypeDef dacInitStruct;
	DAC_InitChannel_TypeDef channelInitStruct;
	CMU_ClockEnable(cmuClock_DAC0,true);
	dacInitStruct.refresh=dacRefresh8;
	dacInitStruct.reference=dacRefVDD;
	dacInitStruct.outMode=dacOutputPin;
	dacInitStruct.convMode=dacConvModeContinuous;
	dacInitStruct.prescale=8; //375 kHz
	dacInitStruct.lpEnable=false;
	dacInitStruct.ch0ResetPre=false;
	dacInitStruct.outEnablePRS=false;
	dacInitStruct.sineEnable=false;
	dacInitStruct.diff=false;
	DAC_Init(DAC0,&dacInitStruct);
	channelInitStruct.enable=false;
	channelInitStruct.prsEnable=false;
	channelInitStruct.prsSel=dacPRSSELCh0;
	channelInitStruct.refreshEnable=false;
	DAC_InitChannel(DAC0,&channelInitStruct,0);
	DAC_Enable(DAC0,0,true);
	dacWrite(0);
}

void dacWrite(uint16_t val){
	DAC0->CH0DATA=val;
}
