#include "ad1835.h"

/* ezekkel a parameterekkel inicializalodik a codec */
const unsigned pm initParams[]={
	WR | DACCTRL1 | DACI2S | DAC24BIT | DACFS48,
	WR | DACCTRL2,
	WR | DACVOL_L1 | DACVOL_MAX,
	WR | DACVOL_R1 | DACVOL_MAX,
	WR | DACVOL_L2 | DACVOL_MAX,
	WR | DACVOL_R2 | DACVOL_MAX,
	WR | DACVOL_L3 | DACVOL_MAX,
	WR | DACVOL_R3 | DACVOL_MAX,
	WR | DACVOL_L4 | DACVOL_MAX,
	WR | DACVOL_R4 | DACVOL_MAX,
	WR | ADCCTRL1 | ADCFS48,
	WR | ADCCTRL2 | ADCI2S | ADC24BIT,
	WR | ADCCTRL3 | IMCLKx2
};

/* ki- es bemenetek */
float rightIn  , leftIn;
float rightOut1, leftOut1;
float rightOut2, leftOut2;
float rightOut3, leftOut3;
float rightOut4, leftOut4;
float rightInD  , leftInD;
float rightOut1D, leftOut1D;
float rightOut2D, leftOut2D;
float rightOut3D, leftOut3D;
float rightOut4D, leftOut4D;
int rightIn_i  , leftIn_i;
int rightOut1_i, leftOut1_i;
int rightOut2_i, leftOut2_i;
int rightOut3_i, leftOut3_i;
int rightOut4_i, leftOut4_i;

/* folyamatban levo feldolgozast es uj adat erkezeset jelzo flagek */
unsigned char isProcessing=0, dataReady=0;

/* 3 Transfer Control Block a DMA chaininghez (az egyikbe vesz, a
   masikat feldolgozza, a harmadikat kuldi, es minden utemben forog) */
unsigned Block_A[2];
unsigned Block_B[2];
unsigned Block_C[2];
int TCB_Block_A[4]={0,sizeof(Block_A),1,0};
int TCB_Block_B[4]={0,sizeof(Block_B),1,0};
int TCB_Block_C[4]={0,sizeof(Block_C),1,0};

/* konfiguralja a soros portokat */
void initSPORT(){
	unsigned PCI=0x00080000;
	unsigned OFFSET=0x00080000;
	TCB_Block_A[0]=(int)TCB_Block_A+3-OFFSET+PCI;
	TCB_Block_A[3]=(unsigned)Block_A-OFFSET;
	*pSPMCTL01=0;
	*pSPMCTL23=0;
	*pSPCTL0=0;
	*pSPCTL1=0;
	*pSPCTL2=0;
	#ifdef DAC1
		*pSPCTL1=(SPTRAN|OPMODE|SLEN24|SPEN_A|BHD|DTYPE1);
		*pTXSP1A=10;
		*pTXSP1A=10;
	#endif
	#ifdef DAC2
		*pSPCTL1|=(SPTRAN|OPMODE|SLEN24|SPEN_B|BHD|DTYPE1);
		*pTXSP1B=0;
		*pTXSP1B=0;
	#endif
	#ifdef DAC3
		*pSPCTL2=(SPTRAN|OPMODE|SLEN24|SPEN_A|BHD|DTYPE1);
		*pTXSP2A=0;
		*pTXSP2A=0;
	#endif
	#ifdef DAC4
		*pSPCTL2|=(SPTRAN|OPMODE|SLEN24|SPEN_B|BHD|DTYPE1);
		*pTXSP2B=0;
		*pTXSP2B=0;
	#endif
	*pSPCTL0=(BHD|OPMODE|SLEN24|SPEN_A|SCHEN_A|SDEN_A|DTYPE1);
	*pCPSP0A=(unsigned)TCB_Block_A-OFFSET+3+PCI;
}

/* beallitja az SRU-t a codec bekotesenek megfeleloen */
void initDAI(){
	SRU(LOW,DAI_PB17_I);
	SRU(LOW,PBEN17_I);
	SRU(DAI_PB07_O,SPORT0_CLK_I);
	SRU(DAI_PB08_O,SPORT0_FS_I);
	SRU(DAI_PB05_O,SPORT0_DA_I);
	SRU(LOW,DAI_PB05_I);
	SRU(LOW,DAI_PB07_I);
	SRU(LOW,DAI_PB08_I);
	SRU(LOW,PBEN05_I);
	SRU(LOW,PBEN07_I);
	SRU(LOW,PBEN08_I);
	SRU(SPORT2_DB_O,DAI_PB09_I);
	SRU(SPORT2_DA_O,DAI_PB10_I);
	SRU(SPORT1_DB_O,DAI_PB11_I);
	SRU(SPORT1_DA_O,DAI_PB12_I);
	SRU(DAI_PB07_O,DAI_PB13_I);
	SRU(DAI_PB08_O,DAI_PB14_I);
	SRU(DAI_PB17_O,DAI_PB06_I);
	SRU(DAI_PB07_O,SPORT1_CLK_I);
	SRU(DAI_PB07_O,SPORT2_CLK_I);
	SRU(DAI_PB08_O,SPORT1_FS_I);
	SRU(DAI_PB08_O,SPORT2_FS_I);
	SRU(HIGH,PBEN06_I);
	SRU(HIGH,PBEN09_I);
	SRU(HIGH,PBEN10_I);
	SRU(HIGH,PBEN11_I);
	SRU(HIGH,PBEN12_I);
	SRU(HIGH,PBEN13_I);
	SRU(HIGH,PBEN14_I);
}

/* inicializalja az SPI portot */
void spiInit(){
	*pSPICTL=(TXFLSH|RXFLSH);
	*pSPIFLG=0;
	*pSPIBAUD=100; //1 MHz
	*pSPIFLG=0xF708;
	*pSPICTL=(SPIEN|SPIMS|MSBF|WL16|TIMOD1);
}

/* letiltja az SPI portot */
void spiDisable(){
	*pSPICTL=(TXFLSH|RXFLSH);
}

/* elküld egy adatot az SPI porton, es visszaadja a vett erteket */
unsigned spiTransfer(unsigned val){
	int i;
	*pTXSPI=val;
	for(i=0;i<100;i++) asm("nop;");
	while(1) if(SPIF&(*pSPISTAT)) break;
	for(i=0;i<100;i++) asm("nop;");
	return *pRXSPI;
}

/* konfiguralja a codec regisztereit (csak ezt kell hivni) */
void initCodec(void){
	int i, configSize=sizeof(initParams)/sizeof(int);
	initSPORT();
	initDAI();
	spiInit();
	for (i=0;i<configSize;++i) spiTransfer(initParams[i]);
	spiDisable();
	interrupt(SIG_SP0,receive);
}

#pragma optimize_for_speed
#define SATURATE(in,sat) if((in)>(sat)) in=sat; if((in)<(-sat)) in=-sat;
#define SCALE (16777216/2)
bool channelCheck;
int mode1RegSPisr, mode1RegSPisr2;

/* elvegzi az adatok vetelet es skalazasat */
void receive(int sig_int){
	channelCheck=!!((*pDAI_PIN_STAT)&DAI_PB14);
	if(channelCheck){
		#ifdef DAC2
			*pTXSP1B=rightOut2_i;
			*pTXSP1B=leftOut2_i;
		#endif
		#ifdef DAC3
			*pTXSP2A=rightOut3_i;
			*pTXSP2A=leftOut3_i;
		#endif
		#ifdef DAC4
			*pTXSP2B=rightOut4_i;
			*pTXSP2B=leftOut4_i;
		#endif	
		#ifdef DAC1
			*pTXSP1A=rightOut1_i;
			*pTXSP1A=leftOut1_i;
		#endif
	}
	else{
		#ifdef DAC2
			*pTXSP1B=leftOut2_i;
			*pTXSP1B=rightOut2_i;
		#endif
		#ifdef DAC3
			*pTXSP2A=leftOut3_i;
			*pTXSP2A=rightOut3_i;
		#endif
		#ifdef DAC4
			*pTXSP2B=leftOut4_i;
			*pTXSP2B=rightOut4_i;
		#endif	
		#ifdef DAC1
			*pTXSP1A=leftOut1_i;
			*pTXSP1A=rightOut1_i;
		#endif
	}
	if(1==isProcessing){
			asm volatile(
			 		"%0=mode1;\n\t"
			 		: "=d" (mode1RegSPisr): :);
	}
	if(0==isProcessing){
			asm volatile(
			 		"%0=mode1;\n\t"
			 		: "=d" (mode1RegSPisr2): :);
	}
	if(channelCheck){
		rightIn_i=(int)Block_A[0];
		leftIn_i =(int)Block_A[1];
	}
	else{
		leftIn_i =(int)Block_A[0];
	 	rightIn_i=(int)Block_A[1];
	}
	rightIn=(float)rightIn_i/(float)SCALE;
	leftIn =(float)leftIn_i /(float)SCALE;
	rightOut1D=rightOut1;
	leftOut1D =leftOut1;
	rightOut2D=rightOut2;
	leftOut2D =leftOut2;
	rightOut3D=rightOut3;
	leftOut3D =leftOut3;
	rightOut4D=rightOut4;
	leftOut4D =leftOut4;
	rightOut1_i=(unsigned)(rightOut1D*SCALE);
	leftOut1_i =(unsigned)(leftOut1D *SCALE);
	rightOut2_i=(unsigned)(rightOut2D*SCALE);
	leftOut2_i =(unsigned)(leftOut2D *SCALE);
	rightOut3_i=(unsigned)(rightOut3D*SCALE);
	leftOut3_i =(unsigned)(leftOut3D *SCALE);
	rightOut4_i=(unsigned)(rightOut4D*SCALE);
	leftOut4_i =(unsigned)(leftOut4D *SCALE);	 		
	SATURATE(rightOut1_i,0x007FFFFF);
	SATURATE( leftOut1_i,0x007FFFFF);
	dataReady=1;
}
