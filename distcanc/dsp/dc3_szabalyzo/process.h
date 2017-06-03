#ifndef _PROCESS_H_
#define _PROCESS_H_

	#include <math.h>
	#include "bsp/ad1835.h"

	#define PI 3.141592653589793
	#define FS 48000.0 //mintaveteli frekvencia
	#define F0 1600.0 //alapharmonikus frekvenciaja
	#define N 10 //megfigyelendo komponensek szama DC-t is beleertve (>=2)
	#define ALPHA (0.05/(N-1)) //megfigyelo csatornak erositese
	#define BETA 0.0001 //szabalyzo csatornak erositese

	/* sajat tipus a komplex szamoknak */
	typedef struct{float re, im;} complex;

	/* a szabalyozo egyes csatornainak atvitele (erosito atvitelenek reciproka * BETA) */
	extern complex beta[N]; 

	/* identifikacio (i-edik harmonikus, samples darab minta) */
	void ident(unsigned i, unsigned samples, complex* transfer);
	
	/* a feldolgozast vegzo fuggveny (megszakitasosan hivodik) */
	void process(void);

#endif
