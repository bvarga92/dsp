#ifndef _PROCESS_H_
#define _PROCESS_H_

	#define PI 3.141592653589793
	#define FS     64000.0 //DSP mintaveteli frekvencia
	#define FLO   100000.0 //lokaljelek frekvenciaja
	#define FIN   101000.0 //bemeno jel frekvenciaja
	#define ALPHA 0.01     //megfigyelo batorsagi tenyezoje
	#define BETA  0.001    //szabalyzo batorsagi tenyezoje
	
	typedef struct{float re, im;} complex;
	
	void ident(void);
	void process(void);

#endif
