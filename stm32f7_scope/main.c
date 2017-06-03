#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_cortex.h"
#include "system.h"
#include "lcd.h"
#include "courier13_ext.h"
#include "graphics.h"
#include "audio.h"
#include <math.h>

#define USE_MIC        1
#define BLOCK_SIZE     800
#define NFFT           256
#define LOG2_NFFT      8
#define WAVE_POS       100
#define MAX_AMP        500

typedef struct{float re,im;} cpx;

int16_t inBuffer[BLOCK_SIZE*2] __attribute__((aligned(4)));
int16_t outBuffer[BLOCK_SIZE*2] __attribute__((aligned(4)));
int16_t dinL[BLOCK_SIZE], dinR[BLOCK_SIZE];
cpx twiddle[NFFT/2];
float window[NFFT];
uint8_t trigCh=0;

void fftInit(cpx* twiddle, float* blackmanWindow){
	uint16_t i;
	float sum;
	for(i=0;i<NFFT/2;i++){
		twiddle[i].re=cosf(6.28319f*i/(float)NFFT);
		twiddle[i].im=-sinf(6.28319f*i/(float)NFFT);
	}
	sum=0;
	for(i=0;i<NFFT;i++){
		blackmanWindow[i]=0.4266f-0.4966f*cosf(6.28319f*i/((float)(NFFT-1)))+0.0768f*cosf(12.56637f*i/((float)(NFFT-1)));
		sum+=blackmanWindow[i];
	}
	for(i=0;i<NFFT;i++) blackmanWindow[i]=blackmanWindow[i]*NFFT/sum;
}

void fft(float* input, cpx* output, cpx* twiddle){
	uint16_t i, irev, j, i1, i2, itw;
	cpx temp1, temp2;
	/* bitreverse cimzes */
	for(i=0;i<NFFT;i++){
		irev=i;
		for(j=0;j<LOG2_NFFT-1;j++) irev=((irev<<1)&(NFFT-1))|((i>>(j+1))&1);
		output[irev].re=input[i];
		output[irev].im=0;
	}
	/* nonrekurziv FFT */
	for(i=0;i<LOG2_NFFT;i++) //hanyadik FFT szint
		for(j=0;j<NFFT/2;j++){ //szinten belul hanyadik butterfly muvelet
			i1=((j<<(i+1))|(j>>(LOG2_NFFT-i-1)))&(NFFT-1); //egyik bemenet indexe
			i2=((j<<(i+1))|(1<<i)|(j>>(LOG2_NFFT-i-1)))&(NFFT-1); //masik bemenet indexe
			itw=(j>>(LOG2_NFFT-i-1))<<(LOG2_NFFT-i-1); //egyutthato indexe
			temp1.re=output[i1].re+twiddle[itw].re*output[i2].re-twiddle[itw].im*output[i2].im;
			temp1.im=output[i1].im+twiddle[itw].re*output[i2].im+twiddle[itw].im*output[i2].re;
			temp2.re=output[i1].re-twiddle[itw].re*output[i2].re+twiddle[itw].im*output[i2].im;
			temp2.im=output[i1].im-twiddle[itw].re*output[i2].im-twiddle[itw].im*output[i2].re;
			output[i1]=temp1;
			output[i2]=temp2;
		}
}

void drawSpectrum(void){
	float input[NFFT];
	cpx spect[NFFT];
	uint16_t i;
	for(i=0;i<NFFT;i++) input[i]=(dinL[i]+dinR[i])*window[i];
	fft(input,spect,twiddle);
	for(i=0;i<NFFT/2-1;i++){
		float val=hypotf(spect[i].re,spect[i].im);
		val=(val<=10)?(1.0f):(logf(val)*10);
		gDrawFilledRectangle(i*3+48,271-val,2,val,COLOR_BLACK);
	}
}

void drawWaveform(void){
	uint16_t i, trigPos;
	uint8_t found=0;
	trigPos=0;
	for(i=0;i<BLOCK_SIZE-480;i++){
		int16_t din=trigCh?dinR[i]:dinL[i];
		if(found==0){
			if(din<-300) found=1;
		}
		else if(din>0){
			trigPos=i;
			break;
		}
	}
	if(trigCh)
		gPutString(5,5,"Trig: R",courier13_ext,13,COLOR_BLACK,COLOR_WHITE);
	else
		gPutString(5,5,"Trig: L",courier13_ext,13,COLOR_BLACK,COLOR_WHITE);
	for(i=0;i<480;i++){
		gDrawLine(i, WAVE_POS-dinL[i+trigPos]*MAX_AMP/32768.0, i+1, WAVE_POS-dinL[i+1+trigPos]*MAX_AMP/32768.0, COLOR_RED);
		gDrawLine(i, WAVE_POS-dinR[i+trigPos]*MAX_AMP/32768.0, i+1, WAVE_POS-dinR[i+1+trigPos]*MAX_AMP/32768.0, COLOR_BLUE);
	}
}

int main(void){
	uint16_t i;
	uint8_t btnStatus=0;
	configMPU();
	enableCache();
	HAL_Init();
	configSystemClock();
	ledInit();
	buttonInit();
	lcdInit();
	lcdInitLayers();
	lcdSetLayer2();
	gFill(COLOR_RED);
	fftInit(twiddle,window);
	#if USE_MIC
		audioInOutInit(INPUT_DEVICE_DIGITAL_MICROPHONE_2,OUTPUT_DEVICE_HEADPHONE,90,AUDIO_FREQUENCY_44K);
	#else
		audioInOutInit(INPUT_DEVICE_INPUT_LINE_1,OUTPUT_DEVICE_HEADPHONE,64,AUDIO_FREQUENCY_44K);
	#endif
	audioSetInputBuffer((uint16_t*)inBuffer,BLOCK_SIZE*2);
	audioSetOutputBuffer((uint16_t*)outBuffer,BLOCK_SIZE*2);
	while(1){
		/* megtelt a buffer elso fele */
		if(inTransferHalfComplete){
			ledOn();
			inTransferHalfComplete=0;
			SCB_InvalidateDCache_by_Addr((uint32_t*)inBuffer,BLOCK_SIZE*2);
			for(i=0;i<BLOCK_SIZE/2;i++){
				dinL[i]=inBuffer[i*2];
				dinR[i]=inBuffer[i*2+1];
				outBuffer[i*2]=inBuffer[i*2];
				outBuffer[i*2+1]=inBuffer[i*2+1];
			}
			ledOff();
		}
		/* megtelt a buffer masodik fele */
		if(inTransferComplete){
			ledOn();
			inTransferComplete=0;
			SCB_InvalidateDCache_by_Addr((uint32_t*)inBuffer,BLOCK_SIZE*4);
			for(i=BLOCK_SIZE/2;i<BLOCK_SIZE;i++){
				dinL[i]=inBuffer[i*2];
				dinR[i]=inBuffer[i*2+1];
				outBuffer[i*2]=inBuffer[i*2];
				outBuffer[i*2+1]=inBuffer[i*2+1];
			}
			gFill(COLOR_WHITE);
			drawSpectrum();
			drawWaveform();
			lcdSwap();
			ledOff();
		}
		/* trigger forras valtogatasa gombnyomasra */
		if(buttonRead()&&(btnStatus==0)){
			btnStatus=1;
			trigCh=~trigCh;
		}
		else if((btnStatus==1)&&(!buttonRead())){
			btnStatus=0;
		}
	}
	return 0;
}
