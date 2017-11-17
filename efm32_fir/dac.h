#ifndef _DAC_H_
#define _DAC_H_

	#include "stdbool.h"
	#include "stdint.h"

	void dacInit(void);
	void dacWrite(uint16_t val);

#endif
