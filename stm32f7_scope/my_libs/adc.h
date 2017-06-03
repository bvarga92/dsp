#ifndef _ADC_H_
#define _ADC_H_

	#include "stm32f7xx.h"
	#include "stm32f7xx_hal.h"
	#include "stm32f7xx_hal_cortex.h"
	#include "stm32f7xx_hal_adc.h"

	/* Inicializalja az ADC1 0. csatornajat (PA0 lab). */
	void adcInit(void);

	/* Elinditja a konverziot. */
	void adcStart(void);

	/* Blokkolva var a konverzio vegere, es visszaadja az erteket. */
	uint16_t adcGet(void);

#endif
