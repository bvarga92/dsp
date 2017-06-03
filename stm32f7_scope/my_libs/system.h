#ifndef _SYSTEM_H_
#define _SYSTEM_H_

	#include "stm32f7xx.h"
	#include "stm32f7xx_hal.h"
	#include "stm32f7xx_hal_cortex.h"

	/* Memory Protection Unit beallitasa (SRAM1+SRAM2: 256 kB). */
	void configMPU(void);

	/* Adat es utasitas cache engedelyezese. */
	void enableCache(void);

	/* Orajelek beallitasa (200 MHz AHB orajel). */
	void configSystemClock(void);

	/* A zold LED-et (LD1) kezelo fuggvenyek. */
	void ledInit(void);
	void ledOn(void);
	void ledOff(void);
	void ledToggle(void);

	/* A kek nyomogombot kezelo fuggvenyek. */
	void buttonInit(void);
	uint8_t buttonRead(void);

#endif
