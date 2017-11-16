#include "adc.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_adc.h"

void adcInit(void){
	ADC_Init_TypeDef adcInitStruct;
	ADC_InitSingle_TypeDef adcInitSingleStruct;
	CMU_ClockEnable(cmuClock_ADC0,true);
	GPIO_PinModeSet(gpioPortD,5,gpioModeInput,0);
	adcInitStruct.timebase=24;
	adcInitStruct.prescale=1; //12 MHz
	adcInitStruct.lpfMode=adcLPFilterBypass;
	adcInitStruct.ovsRateSel=adcOvsRateSel2;
	adcInitStruct.tailgate=false;
	adcInitStruct.warmUpMode=adcWarmupNormal;
	ADC_Init(ADC0,&adcInitStruct);
	adcInitSingleStruct.acqTime=adcAcqTime1;
	adcInitSingleStruct.diff=false;
	adcInitSingleStruct.input=adcSingleInputCh5;
	adcInitSingleStruct.leftAdjust=false;
	adcInitSingleStruct.prsEnable=false;
	adcInitSingleStruct.prsSel=adcPRSSELCh0;
	adcInitSingleStruct.reference=adcRefVDD;
	adcInitSingleStruct.rep=false;
	adcInitSingleStruct.resolution=adcRes12Bit;
	ADC_InitSingle(ADC0,&adcInitSingleStruct);
}

void adcStart(void){
	ADC_Start(ADC0,adcStartSingle);
}

uint16_t adcRead(void){
	while(!((ADC0->STATUS)&ADC_STATUS_SINGLEDV)) ;
	return ADC_DataSingleGet(ADC0);
}
