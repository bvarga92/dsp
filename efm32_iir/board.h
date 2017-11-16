#ifndef _BOARD_H_
#define _BOARD_H_

	#include "stdbool.h"
	#include "stdint.h"

	void boardInit(void);
	void ledOn(uint8_t n);
	void ledOff(uint8_t n);
	void ledToggle(uint8_t n);
	bool ledGet(uint8_t n);
	bool pbGet(uint8_t n);

#endif
