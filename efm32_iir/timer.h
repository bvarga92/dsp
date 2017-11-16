#ifndef _TIMER_H_
#define _TIMER_H_

	#include "stdbool.h"
	#include "stdint.h"

	extern volatile bool timerIntrp;

	void timerInit(uint32_t top);

#endif
