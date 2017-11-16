#ifndef _ADC_H_
#define _ADC_H_

	#include "stdbool.h"
	#include "stdint.h"

	void adcInit(void);
	void adcStart(void);
	uint16_t adcRead(void);

#endif
