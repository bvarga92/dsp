/* Driver a DSP panelen talalhato audio codechez. */

#ifndef _AD1835_H_
#define _AD1835_H_

	#include <cdef21364.h>
	#include <def21364.h>
	#include <signal.h>
	#include <SRU21364.h>

	/* amelyik DAC csatornara nincs szukseg, azt kommentezzuk ki */
	#define DAC1
	#define DAC2
	#define DAC3
	#define DAC4

	/* ki- es bemenetek */
	extern float rightIn  , leftIn;
	extern float rightOut1, leftOut1;
	extern float rightOut2, leftOut2;
	extern float rightOut3, leftOut3;
	extern float rightOut4, leftOut4;

	/* folyamatban levo feldolgozast es uj adat erkezeset jelzo flagek */
	extern unsigned char isProcessing, dataReady;

	/* fuggvenyprototipusok */
	void initSPORT(void);
	void initDAI(void);
	void spiInit(void);
	void spiDisable(void);
	unsigned spiTransfer(unsigned val);
	void initCodec(void);
	void receive(int sig_int);

	/* a codec regiszterei */

	#define DACCTRL1    (0x0000)  // DAC control register 1    (R/W)
	#define DACCTRL2    (0x1000)  // DAC control register 2    (R/W)
	#define DACVOL_L1   (0x2000)  // DAC volume - left 1       (R/W)
	#define DACVOL_R1   (0x3000)  // DAC volume - right 1      (R/W)
	#define DACVOL_L2   (0x4000)  // DAC volume - left 2       (R/W)
	#define DACVOL_R2   (0x5000)  // DAC volume - right 2      (R/W)
	#define DACVOL_L3   (0x6000)  // DAC volume - left 3       (R/W)
	#define DACVOL_R3   (0x7000)  // DAC volume - right 3      (R/W)
	#define DACVOL_L4   (0x8000)  // DAC volume - left 4       (R/W)
	#define DACVOL_R4   (0x9000)  // DAC volume - right 4      (R/W)
	#define ADCPEAKL    (0xA000)  // ADC left peak             (R)
	#define ADCPEAKR    (0xB000)  // ADC right peak            (R)
	#define ADCCTRL1    (0xC000)  // ADC control 1             (R/W)
	#define ADCCTRL2    (0xD000)  // ADC control 2             (R/W)
	#define ADCCTRL3    (0xE000)  // ADC control 3             (R/W)

	#define RD          (0x0800)
	#define WR          (0x0000)  // Write to register

	// DAC control register 1
	#define DEEMPH44_1  (0x0100)  // Deemphasis filter for 44.1 KHz
	#define DEEMPH32    (0x0200)  // Deemphasis filter for 32.0 KHz
	#define DEEMPH48    (0x0300)  // Deemphasis filter for 48.0 KHz

	#define DACI2S      (0x0000)  // DAC receives I2S format
	#define DACRJ       (0x0020)  // DAC receives I2S format
	#define DACDSP      (0x0040)  // DAC receives I2S format
	#define DACLJ       (0x0060)  // DAC receives I2S format
	#define DACPACK256  (0x0080)  // DAC receives I2S format

	#define DAC24BIT    (0x0000)  // 24-bit output word length
	#define DAC20BIT    (0x0008)  // 20-bit output word length
	#define DAC16BIT    (0x0010)  // 16-bit output word length

	#define DACPOWERDN  (0x0004)  // DAC into power-down mode

	#define DACFS48     (0x0000)  // Sample rate = 48 KHz (x8)
	#define DACFS96     (0x0001)  // Sample rate = 96 KHz (x4)
	#define DACFS192    (0x0002)  // Sample rate = 192 KHz (x2)

	// DAC control register 2
	#define DACREPLICATE  (0x0100)  // Replicate output of DAC 1/2 on 3/4, 5/6 & 7/8
	#define DACMUTE_R4    (0x0080)  // Mute DAC output channel (clear to un-mute)
	#define DACMUTE_L4    (0x0040)  // Mute DAC output channel (clear to un-mute)
	#define DACMUTE_R3    (0x0020)  // Mute DAC output channel (clear to un-mute)
	#define DACMUTE_L3    (0x0010)  // Mute DAC output channel (clear to un-mute)
	#define DACMUTE_R2    (0x0008)  // Mute DAC output channel (clear to un-mute)
	#define DACMUTE_L2    (0x0004)  // Mute DAC output channel (clear to un-mute)
	#define DACMUTE_R1    (0x0002)  // Mute DAC output channel (clear to un-mute)
	#define DACMUTE_L1    (0x0001)  // Mute DAC output channel (clear to un-mute)

	//DAC Volume Control - 10-bit granularity (1024 levels)
	#define DACVOL_MIN      (0x000)
	#define DACVOL_LOW      (0X100)
	#define DACVOL_MED      (0X200)
	#define DACVOL_HI       (0X300)
	#define DACVOL_MAX      (0x3FF)
	#define DACVOL_MASK     (0x3FF)  // Volume in dB is in 10 LSBs
	                                 //   3FF = 0 dBFS = 1023/1023
	                                 //   3FE = -0.01 dBFS = 1022/1023
	                                 //      ...
	                                 //   002 = -50.7 dBFS = 3/1023
	                                 //   001 = -54.2 dBFS = 2/1023

	//  ADC Control 1
	#define ADCHPF     (0x0100)  // High pass filter (AC-coupled)
	#define ADCPOWERDN (0x0080)  // DAC into power-down mode
	#define ADCFS48    (0x0000)  // Sample rate = 48 KHz
	#define ADCFS96    (0x0001)  // Sample rate = 96 KHz

	//  ADC Control 2
	#define AUXSLAVE   (0x0000)  // Aux input is in slave mode
	#define AUXMASTER  (0x0200)  // Aux input is in master mode

	#define ADCI2S     (0x0000)  // ADC transmits in I2S format
	#define ADCRJ      (0x0040)  // ADC transmits in right-justified format
	#define ADCDSP     (0x0080)  // ADC transmits in DSP (TDM) format
	#define ADCLJ      (0x00C0)  // ADC transmits in left-justified format
	#define ADCPACK256 (0x0100)  // ADC transmits in packed 256 format
	#define ADCAUX256  (0x0180)  // ADC transmits in packed 128 format

	#define ADC24BIT   (0x0000)  // 24-bit output word length
	#define ADC20BIT   (0x0010)  // 20-bit output word length
	#define ADC16BIT   (0x0020)  // 16-bit output word length

	#define ADCMUTER   (0x0002)  // Mute right channel from ADC
	#define ADCMUTEL   (0x0001)  // Mute right channel from ADC

	//  ADC Control 3
	#define IMCLKx2    (0x0000)  // Internal MCLK = external MCLK x 2
	#define IMCLKx1    (0x0080)  // Internal MCLK = external MCLK
	#define IMCLKx23   (0x0100)  // Internal MCLK = external MCLK x 2/3

	#define PEAKRDEN   (0x0020)  // Enable reads of peak ADC levels

#endif
