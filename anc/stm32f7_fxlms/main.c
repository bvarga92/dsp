#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_cortex.h"
#include "system.h"
#include "lcd.h"
#include "courier20b_ext.h"
#include "courier13_ext.h"
#include "graphics.h"
#include "audio.h"
#include "adc.h"
#include <stdlib.h>
#include <math.h>

/* parameterek */
#define FS           AUDIO_FREQUENCY_16K
#define N_S          140
#define N_W          60
#define IDENT_SCALE  0.5f
#define MU           0.0001f

/* makrok a tipuskonverziokhoz */
#define FLOAT_SAT(f) (((f)<-1.0f)?(-1.0f):(((f)>1.0f)?(1.0f):(f)))
#define FLOAT_TO_I16(f)	((int16_t)((f)*32767))
#define I16_TO_FLOAT(i)	((float)((i)/32767.0f))
#define U12_TO_FLOAT(u) ((float)((u)/2047.5f-1.0f))

/* globalis valtozok */
float S[N_S];
float W[N_W];
int16_t inBuffer[2] __attribute__((aligned(4)));
int16_t outBuffer[2] __attribute__((aligned(4)));
int16_t micL, outR;

/* vegrehajt egy identifikacios utemet */
void ident(void){
	static float y_buf[N_S];
	float y, e;
	static uint16_t i=0;
	uint16_t j, ix;
	e=I16_TO_FLOAT(micL);
	y=(rand()/(float)RAND_MAX-0.5f)*IDENT_SCALE;
	outR=FLOAT_TO_I16(y);
	y_buf[i]=y;
	ix=i;
	for(j=0;j<N_S;j++,ix++) e-=y_buf[ix%N_S]*S[j];
	e*=MU;
	for(j=0;j<N_S;j++) S[j]+=e*y_buf[(j+i)%N_S];
	i=i?(i-1):(N_S-1);
}

/* kirajzolja az identifikalt atvitelt */
void plotS(uint16_t xStride, uint16_t xMin){
	float min, max, scale, mu_m;
	uint16_t i, prev, cur;
	uint8_t mu_e;
	min=max=S[0];
	char str[20];
	for(i=1;i<N_S;i++){
		if(S[i]<min) min=S[i];
		if(S[i]>max) max=S[i];
	}
	scale=251.0f/(max-min);
	gFill(COLOR_WHITE);
	for(i=1;i<N_S;i++){
		prev=261-(uint16_t)((S[i-1]-min)*scale);
		cur=261-(uint16_t)((S[i]-min)*scale);
		gDrawLine((i-1)*xStride+xMin,prev,i*xStride+xMin,cur,COLOR_BLACK);
	}
	gDrawRectangle(xMin,10,N_S*xStride-1,252,COLOR_RED);
	sprintf(str,"fs=%u kHz",(uint16_t)(FS/1000));
	gPutString(N_S*xStride+15,10,str,courier13_ext,13,COLOR_BLACK,COLOR_WHITE);
	sprintf(str,"NS=%u",N_S);
	gPutString(N_S*xStride+15,30,str,courier13_ext,13,COLOR_BLACK,COLOR_WHITE);
	sprintf(str,"NW=%u",N_W);
	gPutString(N_S*xStride+15,50,str,courier13_ext,13,COLOR_BLACK,COLOR_WHITE);
	for(mu_e=0,mu_m=MU ; roundf(mu_m)<1 ; ){mu_m*=10; mu_e++;}
	sprintf(str,"mu=%de-%d",(int)roundf(mu_m),mu_e);
	gPutString(N_S*xStride+15,70,str,courier13_ext,13,COLOR_BLACK,COLOR_WHITE);
}

/* vegrehajtja az FxLMS algoritmus egy utemet */
void fxlms(void){
	static float x_buf_W[N_W], x_buf_S[N_S], r_buf[N_W];
	static uint16_t iW=0, iS=0;
	uint16_t i, j;
	float e, y, r;
	e=MU*I16_TO_FLOAT(micL);
	if(buttonRead()){outR=0; adcStart(); return;}; //gombnyomasra szunetel a zajcsokkentes
	/* y=-W*x */
	x_buf_W[iW]=U12_TO_FLOAT(adcGet());
	i=iW;
	y=0;
	for(j=0;j<N_W;j++,i++) y-=x_buf_W[i%N_W]*W[j];
	/* r=S*x */
	x_buf_S[iS]=x_buf_W[iW];
	i=iS;
	r=0;
	for(j=0;j<N_S;j++,i++) r+=x_buf_S[i%N_S]*S[j];
	/* dW=mu*e*R */
	r_buf[iW]=r;
	for(j=0;j<N_W;j++) W[j]+=e*r_buf[(j+iW)%N_W];
	/* frissites a kovetkezo utemre */
	iW=iW?(iW-1):(N_W-1);
	iS=iS?(iS-1):(N_S-1);
	outR=FLOAT_TO_I16(FLOAT_SAT(y));
	adcStart();
}

/* foprogram */
int main(void){
	/* hardver inicializalasa */
	configMPU();
	enableCache();
	HAL_Init();
	configSystemClock();
	ledInit();
	buttonInit();
	lcdInit();
	lcdInitLayers();
	lcdSetLayer2();
	gFill(COLOR_WHITE);
	adcInit();
	audioInOutInit(INPUT_DEVICE_DIGITAL_MICROPHONE_2,OUTPUT_DEVICE_HEADPHONE,100,FS);
	audioSetOutputBuffer((uint16_t*)outBuffer,2);
	audioSetInputBuffer((uint16_t*)inBuffer,2);
	/* identifikacio */
	gPutString(70,110,"Identifik\xE1""ci\xF3""hoz tartsd",courier20b_ext,20,COLOR_BLACK,COLOR_WHITE);
	gPutString(120,140,"nyomva a gombot!",courier20b_ext,20,COLOR_BLACK,COLOR_WHITE);
	while(!buttonRead()) ;
	gFill(COLOR_WHITE);
	gPutString(45,120,"Identifik\xE1""ci\xF3 folyamatban...",courier20b_ext,20,COLOR_BLACK,COLOR_WHITE);
	while(buttonRead()){
		if(inTransferComplete){ //csak a bufferek masodik felet kezeljuk (micL, outR)
			ledOn();
			inTransferComplete=0;
			SCB_InvalidateDCache_by_Addr((uint32_t*)inBuffer,4);
			micL=inBuffer[1];
			outBuffer[1]=outR;
			ident();
			ledOff();
		}
	}
	plotS(2,10);
	/* zajcsokkentes */
	while(1){
		if(inTransferComplete){ //csak a bufferek masodik felet kezeljuk (micL, outR)
			ledOn();
			inTransferComplete=0;
			SCB_InvalidateDCache_by_Addr((uint32_t*)inBuffer,4);
			micL=inBuffer[1];
			outBuffer[1]=outR;
			fxlms();
			ledOff();
		}
	}
	return 0;
}
