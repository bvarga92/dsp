#ifndef _RANDOM_H
#define _RANDOM_H

	#include "stm32f7xx.h"
	#include "stm32f7xx_hal.h"
	#include "stm32f7xx_hal_cortex.h"
	#include "stm32f7xx_hal_rng.h"

	/* Inicializalja a veletlenszam-generatort. */
	void randomInit(void);

	/* Visszaad egy 32 bites veletlenszamot. */
	uint32_t randomGet(void);

#endif
