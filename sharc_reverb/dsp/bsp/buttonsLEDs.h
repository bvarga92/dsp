/* A panelen talalhato 4 gombot es 8 LED-et kezelo fuggvenyek. */

#ifndef _BUTTONSLEDS_H_
#define _BUTTONSLEDS_H_

	#include <cdef21364.h>
	#include <def21364.h>
	#include <SRU21364.h>
	
	#define readSW1() flagTest(FLG1)
	#define readSW2() flagTest(FLG2)
	#define readSW3() readDAIpin(1<<18)
	#define readSW4() readDAIpin(1<<19)

	void initButtons(void);
	int flagTest(int mask);
	int readDAIpin(int mask);
	void writeLEDs(int ledValue);

#endif
