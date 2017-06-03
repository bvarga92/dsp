#ifndef _PROCESS_H_
#define _PROCESS_H_

	#include <math.h>
	#include "bsp/ad1835.h"

	#define PI 3.141592653589793
	#define FS 48000.0 //mintaveteli frekvencia
	#define F0 440.0 //megfigyelendo alapharmonikus nevleges frekvenciaja
	#define N 10 //megfigyelendo komponensek szama DC-t is beleertve (>=2)
	#define ALPHA (0.01/(N-1)) //az egyes csatornak erositese
	
	/* sajat tipus a komplex szamoknak */
	typedef struct{float re, im;} complex;

	/* ezzel kapcsolhatjuk ki-be a megfigyelot */
	extern bool observerOn;

	/* a feldolgozast vegzo fuggveny (megszakitasosan hivodik) */
	void process(void);

#endif
