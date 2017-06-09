#ifndef _TOUCH_H_
#define _TOUCH_H_

	#include "stm32f7xx.h"
	#include "stm32f7xx_hal.h"
	#include "stm32f7xx_hal_cortex.h"
	#include "stm32f7xx_hal_i2c.h"

	/* adattipusok */
	typedef enum{TOUCH_PRESSED,TOUCH_RELEASED} touchStatus_t;
	typedef struct{uint16_t x, y;} point_t;
	typedef struct{
		touchStatus_t status; //van-e erintes
		uint16_t cnt; //hany darab (0-5)
		point_t p[5]; //mely pontokban
	} touchData_t;

	/* az erintesi pontokat tartalmazo globalis valtozo*/
	extern touchData_t touchData;


	/* Inicializalja az erintokepernyot. */
	void touchInit(void);

	/* Frissiti touchData tartalmat. */
	void touchRead(void);

#endif
