#include "iqmixer.h"
#include <cdef21364.h>
#include <def21364.h>
#include <signal.h>
#define SRUDEBUG
#include <SRU21364.h>


float out1, out2, in1, in2;
bool dataReady;


static void delay(unsigned int d);
static void spiTransfer(unsigned char byteToSend);
static void codecISR(int sigInt);


/* varakozociklus keslelteteshez */
static void delay(unsigned int d){
	for(;d;d--) asm("nop;");
}

/* elkuldi a parameterkent kapott bajtot az SPI porton */
static void spiTransfer(unsigned char byteToSend){
	*pTXSPI=byteToSend;
	while(!(*pSPISTAT&SPIF)) ;
	delay(1000);
}

/* AD73322L kodek inicializalasa */
void iqInitCodec(void){
	unsigned int initWords[]={0x8903, 0x8103, 0x8a7e, 0x827e,
	                          0x8b00, 0x8300, 0x8c00, 0x8400,
	                          0x8d1f, 0x851f, 0x8e00, 0x8600,
	                          0x8f80, 0x8780, 0x8811, 0x8011};
	unsigned int i;
	/* kimenetek: DAIP1 (SDI), DAIP9 (RST)
	   bemenetek: DAIP2 (SCLK), DAIP3 (FS), DAIP4 (SDO) bemenet */
	SRU(HIGH,PBEN01_I);
	SRU(HIGH,PBEN09_I);
	SRU(LOW,DAI_PB09_I);
	SRU(LOW,PBEN02_I);
	SRU(LOW,DAI_PB02_I);
	SRU(LOW,PBEN03_I);
	SRU(LOW,DAI_PB03_I);
	SRU(LOW,PBEN04_I);
	SRU(LOW,DAI_PB04_I);
	/* SPORT osszekotesek */
	SRU(SPORT4_DA_O,DAI_PB01_I);
	SRU(DAI_PB02_O,SPORT4_CLK_I);
	SRU(DAI_PB03_O,SPORT4_FS_I);
	SRU(DAI_PB04_O,SPORT5_DA_I);
	SRU(DAI_PB02_O,SPORT5_CLK_I);
	SRU(DAI_PB03_O,SPORT5_FS_I);
	/* SPORT konfiguralasa */
	*pSPCTL4=SPTRAN|FSR|SPEN_A|SLEN16;
	*pSPCTL5=FSR|SPEN_A|SLEN16;
	/* kodek konfiguralasa */
	delay(1000000);
	*pTXSP4A=initWords[0]; // betoltjuk az elso szot a bufferbe...
	SRU(HIGH,DAI_PB09_I); // ...majd kivesszuk resetbol a kodeket
	for(i=1;i<16;i++){
		while((*pSPCTL4)&DXS1_A) ;
		*pTXSP4A=initWords[i];
	}
	delay(1000000);
	interrupt(SIG_SP5,codecISR);
}

/* megszakitaskezelo rutin a kodekhez (lefut, ha minta jott) */
static void codecISR(int sigInt){
	static bool channel=false, ch1Ready=false, ch2Ready=false;
	static float out1_prev=0; // kesleltetes kompenzalasara
	int temp;
	if(channel==false){
		channel=true;
		temp=*pRXSP5A;
		if(temp>=32768) temp-=65536;
		in1=GAIN_IN1*(temp/32768.0-OFF_IN1);
		temp=(int)(out1_prev*32767.0);
		out1_prev=GAIN_OUT1*(out1-OFF_OUT1);
		*pTXSP4A=(temp<0)?temp+65536:temp;
		ch1Ready=true;
	}
	else{
		channel=false;
		temp=*pRXSP5A;
		if(temp>=32768) temp-=65536;
		in2=temp/32768.0-OFF_IN2;
		temp=(int)((out2-OFF_OUT2)*32767.0);
		*pTXSP4A=(temp<0)?temp+65536:temp;
		ch2Ready=true;
	}
	if(ch1Ready&&ch2Ready){
		ch1Ready=ch2Ready=false;
		dataReady=true;
	}
}

/* AD9854 DDS chip inicializalasa */
void iqInitDDS(void){
	/* kimenetek: DAIP10 (MRST), DAIP15 (IOUD), DAIP16 (orajel OE), DAIP19 (IORST) */
	SRU(HIGH,PBEN10_I);
	SRU(HIGH,DAI_PB10_I); // reset
	SRU(HIGH,PBEN15_I);
	SRU(LOW,DAI_PB15_I);
	SRU(HIGH,PBEN16_I);
	SRU(LOW,DAI_PB16_I); // orajel letilt
	SRU(HIGH,PBEN19_I);
	SRU(LOW,DAI_PB19_I);
	/* SPI konfiguralasa: 1MHz, CS=FLAG2, master mod, 8 bit, MSB-first */
	*pSPICTL=TXFLSH|RXFLSH;
	*pSPIBAUD=100;
	*pSPIFLG=0xFB04;
	*pSPICTL=SPIEN|SPIMS|MSBF|WL8|TIMOD1;
	/* Control Register irasa
			31:      0     - x
			30:      0     - x
			29:      0     - x
			28:      1     - komparator ki
			27:      0     - x
			26:      0     - Q DAC be
			25:      0     - minden DAC be
			24:      0     - digitalis reszek be
			23:      0     - x
			22:      1     - PLL VCO erosites
			21:      1     - orajeltobbszorozo PLL ki
			20...16: 00100 - orajel szorzo
			15:      0     - akkumulator 1 torlese ki
			14:      0     - akkumulatorok torlese ki
			13:      0     - sweep ki
			12:      0     - Q jel nem a Q DAC regiszterbol
			11...09: 000   - single-tone mode
			08:      0     - kulso IOUD (fontos!!!)
			07:      0     - x
			06:      1     - sinc-1 szuro ki (30%-kal noveli az aramfelvetelt!)
			05:      0     - shaped keying ki
			04:      0     - shaped keying programozhato
			03:      0     - x
			02:      0     - x
			01:      0     - MSB-first atvitel
			00:      1     - SDO aktiv, egyiranyu adatvonalak (fontos!!!)
	*/
	SRU(LOW,DAI_PB10_I); // kivesszuk resetbol
	delay(1000);
	SRU(HIGH,DAI_PB19_I); // IO reset
	delay(1000);
	SRU(LOW,DAI_PB19_I); // IO reset vissza
	delay(1000);
	spiTransfer(0x07);
	spiTransfer(0x10);
	spiTransfer(0x64);
	spiTransfer(0x00);
	spiTransfer(0x41);
	delay(1000);
	SRU(HIGH,DAI_PB16_I); // indulhat az orajel
}

/* Frequency Tuning Word 1 irasa (fki=FTW1*125000000/2^48 [Hz]) */
void iqSetFTW1(unsigned ftw1U, unsigned ftw1L){
	delay(1000);
	SRU(HIGH,DAI_PB19_I); // IO reset
	delay(1000);
	SRU(LOW,DAI_PB19_I); // IO reset vissza
	delay(1000);
	spiTransfer(0x02);
	spiTransfer((ftw1U>>16)&0xFF);
	spiTransfer((ftw1U>>8)&0xFF);
	spiTransfer(ftw1U&0xFF);
	spiTransfer((ftw1L>>16)&0xFF);
	spiTransfer((ftw1L<<8)&0xFF);
	spiTransfer(ftw1L&0xFF);
	delay(1000);
	SRU(HIGH,DAI_PB15_I); // IO update
	delay(1000);
	SRU(LOW,DAI_PB15_I); // IO update vissza
}

/* frekvencia beallitasa (setFTW1 pontosabb!!!) */
void iqSetDDSFreq(double freqHz){
	unsigned upper, lower;
	upper=((unsigned)(freqHz*0.268435456))>>1;
	lower=(unsigned)(freqHz*2251799.813685248-16777216.0*upper);
	iqSetFTW1(upper,lower);
}
