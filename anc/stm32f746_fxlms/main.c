/************************************************/
/*                                              */
/* Globalis compiler beallitasok:               */
/*   -Ofast                                     */
/*   -DARM_MATH_CM7                             */
/*   -D__FPU_PRESENT                            */
/*                                              */
/************************************************/

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
#include "arm_math.h"

/* parameterek */
#define USE_CMSIS    1
#define FS           AUDIO_FREQUENCY_8K
#define N_S          400
#define N_W          200
#define IDENT_SCALE  0.5f
#define MIC_SCALE    8
#define MU           0.0001f

/* makrok a tipuskonverziokhoz */
#define FLOAT_TO_I16(f)	((int16_t)((f)*32767))
#define I16_TO_FLOAT(i)	((float)((i)/32767.0f))
#define U12_TO_FLOAT(u) ((float)((u)/2047.5f-1.0f))

/* globalis valtozok */
static float S[N_S];
static float W[N_W];
static int16_t  inBuffer[2] __attribute__((aligned(4)));
static int16_t outBuffer[2] __attribute__((aligned(4)));
static float micL, outR;
static arm_lms_instance_f32 lmsS;
static arm_fir_instance_f32 firS, firW;

/* fuggvenyprototipusok */
void cmsisInit(void); //inicializalja a CMSIS-es szuroket
void ident(void); //vegrehajt egy identifikacios utemet
void plotS(uint16_t xStride, uint16_t xMin); //kirajzolja az identifikalt atvitelt
void fxlms(void); //vegrehajtja az FxLMS algoritmus egy utemet
int main(void); //foprogram


void cmsisInit(void){
	#if USE_CMSIS
		static float lmsSState[N_S], firSState[N_S], firWState[N_W];
		arm_lms_init_f32(&lmsS,N_S,(float32_t*)S,(float32_t*)lmsSState,MU,1);
		arm_fir_init_f32(&firS,N_S,(float32_t*)S,(float32_t*)firSState,1);
		arm_fir_init_f32(&firW,N_W,(float32_t*)W,(float32_t*)firWState,1);
	#endif
}

void ident(void){
	#if USE_CMSIS
		float y, yS, e;
		y=(rand()/(float)RAND_MAX-0.5f)*IDENT_SCALE;
		outR=y;
		arm_lms_f32(&lmsS,(float32_t*)&y,(float32_t*)&micL,(float32_t*)&yS,(float32_t*)&e,1);
	#else
		static float y_buf[N_S];
		float y, e;
		static uint16_t i=0;
		uint16_t j, ix;
		e=micL;
		y=(rand()/(float)RAND_MAX-0.5f)*IDENT_SCALE;
		outR=y;
		y_buf[i]=y;
		ix=i;
		for(j=0;j<N_S;j++,ix++) e-=y_buf[ix%N_S]*S[j];
		e*=MU;
		for(j=0;j<N_S;j++) S[j]+=e*y_buf[(j+i)%N_S];
		i=i?(i-1):(N_S-1);
	#endif
}

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
		#if USE_CMSIS //ekkor S forditott sorrendben van
			prev=261-(uint16_t)((S[N_S-i]-min)*scale);
			cur=261-(uint16_t)((S[N_S-i-1]-min)*scale);
		#else
			prev=261-(uint16_t)((S[i-1]-min)*scale);
			cur=261-(uint16_t)((S[i]-min)*scale);
		#endif
		gDrawLine((i-1)*xStride+xMin,prev,i*xStride+xMin,cur,COLOR_BLACK);
	}
	gDrawRectangle(xMin,10,N_S*xStride-1,252,COLOR_RED);
	sprintf(str,"fs=%u kHz",(uint16_t)(FS/1000));
	gPutString(N_S*xStride+xMin+5,10,str,courier13_ext,13,COLOR_BLACK,COLOR_WHITE);
	sprintf(str,"NS=%u",N_S);
	gPutString(N_S*xStride+xMin+5,30,str,courier13_ext,13,COLOR_BLACK,COLOR_WHITE);
	sprintf(str,"NW=%u",N_W);
	gPutString(N_S*xStride+xMin+5,50,str,courier13_ext,13,COLOR_BLACK,COLOR_WHITE);
	for(mu_e=0,mu_m=MU ; roundf(mu_m)<1 ; ){mu_m*=10; mu_e++;}
	sprintf(str,"mu=%de-%d",(int)roundf(mu_m),mu_e);
	gPutString(N_S*xStride+xMin+5,70,str,courier13_ext,13,COLOR_BLACK,COLOR_WHITE);
}

void fxlms(void){
	#if USE_CMSIS
		static float r_buf[N_W];
		static uint16_t iW=0;
		uint16_t j, k;
		float e, y, r, x;
		x=U12_TO_FLOAT(adcGet());
		if(buttonRead()){outR=0; adcStart(); return;}; //gombnyomasra szunetel a zajcsokkentes
		/* r=S*x */
		arm_fir_f32(&firS,(float32_t*)&x,(float32_t*)&r,1);
		/* y=W*x */
		arm_fir_f32(&firW,(float32_t*)&x,(float32_t*)&y,1);
		/* dW=mu*e*R */
		e=MU*micL;
		r_buf[iW]=r;
		k=N_W-1;
		for(j=iW;j<N_W;j++,k--) W[k]+=e*r_buf[j];
		for(j=0;j<iW;j++,k--) W[k]+=e*r_buf[j];
		/* frissites a kovetkezo utemre */
		iW=iW?(iW-1):(N_W-1);
		outR=-y;
		adcStart();
	#else
		static float x_buf_W[N_W], x_buf_S[N_S], r_buf[N_W];
		static uint16_t iW=0, iS=0;
		uint16_t i, j, k;
		float e, y, r, x;
		x=U12_TO_FLOAT(adcGet());
		if(buttonRead()){outR=0; adcStart(); return;}; //gombnyomasra szunetel a zajcsokkentes
		/* r=S*x */
		x_buf_S[iS]=x;
		i=iS;
		r=0;
		for(j=0;j<N_S;j++,i++) r+=x_buf_S[i%N_S]*S[j];
		/* y=W*x */
		x_buf_W[iW]=x;
		i=iW;
		y=0;
		for(j=0;j<N_W;j++,i++) y+=x_buf_W[i%N_W]*W[j];
		/* dW=mu*e*R */
		e=MU*micL;
		r_buf[iW]=r;
		k=0;
		for(j=iW;j<N_W;j++,k++) W[k]+=e*r_buf[j];
		for(j=0;j<iW;j++,k++) W[k]+=e*r_buf[j];
		/* frissites a kovetkezo utemre */
		iW=iW?(iW-1):(N_W-1);
		iS=iS?(iS-1):(N_S-1);
		outR=-y;
		adcStart();
	#endif
}

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
	cmsisInit();
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
			micL=I16_TO_FLOAT(inBuffer[1]*MIC_SCALE);
			outBuffer[1]=FLOAT_TO_I16(outR);
			ident();
			ledOff();
		}
	}
	plotS(1,5);
	/* zajcsokkentes */
	while(1){
		if(inTransferComplete){ //csak a bufferek masodik felet kezeljuk (micL, outR)
			ledOn();
			inTransferComplete=0;
			SCB_InvalidateDCache_by_Addr((uint32_t*)inBuffer,4);
			micL=I16_TO_FLOAT(inBuffer[1]*MIC_SCALE);
			outBuffer[1]=FLOAT_TO_I16(outR);
			fxlms();
			ledOff();
		}
	}
	return 0;
}
