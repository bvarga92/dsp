#ifndef _IQMIXER_H_
#define _IQMIXER_H_

	#define GAIN_OUT1  0.96200
	#define OFF_OUT1   0.01600
	#define OFF_OUT2   0.01000
	#define GAIN_IN1   0.98123
	#define OFF_IN1    0.06835
	#define OFF_IN2    0.05660

	extern float out1, out2, in1, in2;
	extern bool dataReady;

	void iqInitCodec(void);
	void iqInitDDS(void);
	void iqSetFTW1(unsigned ftw1U, unsigned ftw1L);
	void iqSetDDSFreq(double freqHz);

#endif
