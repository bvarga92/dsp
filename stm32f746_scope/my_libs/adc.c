#include "adc.h"

static ADC_HandleTypeDef adcHandle;

void adcInit(void){
	GPIO_InitTypeDef portInit;
	ADC_ChannelConfTypeDef adcConfig;
	__GPIOA_CLK_ENABLE();
	portInit.Pin=GPIO_PIN_0;
	portInit.Mode=GPIO_MODE_ANALOG;
	portInit.Pull=GPIO_NOPULL;
	portInit.Speed=GPIO_SPEED_LOW;
	HAL_GPIO_Init(GPIOA,&portInit);
	__ADC1_CLK_ENABLE();
	adcHandle.Instance=ADC1;
	adcHandle.Init.ClockPrescaler=ADC_CLOCKPRESCALER_PCLK_DIV4;
	adcHandle.Init.Resolution=ADC_RESOLUTION_12B;
	adcHandle.Init.ScanConvMode=DISABLE;
	adcHandle.Init.ContinuousConvMode=DISABLE;
	adcHandle.Init.DiscontinuousConvMode=DISABLE;
	adcHandle.Init.NbrOfDiscConversion=0;
	adcHandle.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE;
	adcHandle.Init.ExternalTrigConv=ADC_EXTERNALTRIGCONV_T1_CC1;
	adcHandle.Init.DataAlign=ADC_DATAALIGN_RIGHT;
	adcHandle.Init.NbrOfConversion=1;
	adcHandle.Init.DMAContinuousRequests=DISABLE;
	adcHandle.Init.EOCSelection=DISABLE;
	HAL_ADC_Init(&adcHandle);
	adcConfig.Channel=ADC_CHANNEL_0;
	adcConfig.Rank=1;
	adcConfig.SamplingTime=ADC_SAMPLETIME_3CYCLES;
	adcConfig.Offset=0;
	HAL_ADC_ConfigChannel(&adcHandle,&adcConfig);
}

void adcStart(void){
	HAL_ADC_Start(&adcHandle);
}

uint16_t adcGet(void){
	HAL_ADC_PollForConversion(&adcHandle,10);
	return HAL_ADC_GetValue(&adcHandle);
}
