#include "lcd.h"
#include "sdram.h"

static LTDC_HandleTypeDef lcdHandle;
uint8_t currentLayer;
uint32_t fbAddr;

void lcdInit(void){
	GPIO_InitTypeDef portInit;
	RCC_PeriphCLKInitTypeDef clkInit;
	sdramInit();
	/* GPIO beallitasa */
	__LTDC_CLK_ENABLE();
	__GPIOE_CLK_ENABLE();
	__GPIOG_CLK_ENABLE();
	__GPIOI_CLK_ENABLE();
	__GPIOJ_CLK_ENABLE();
	__GPIOK_CLK_ENABLE();
	portInit.Pin=GPIO_PIN_4;
	portInit.Mode=GPIO_MODE_AF_PP;
	portInit.Pull=GPIO_NOPULL;
	portInit.Speed=GPIO_SPEED_FAST;
	portInit.Alternate=GPIO_AF14_LTDC;
	HAL_GPIO_Init(GPIOE,&portInit);
	portInit.Pin=GPIO_PIN_12;
	portInit.Mode=GPIO_MODE_AF_PP;
	portInit.Alternate=GPIO_AF9_LTDC;
	HAL_GPIO_Init(GPIOG,&portInit);
	portInit.Pin=GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	portInit.Mode=GPIO_MODE_AF_PP;
	portInit.Alternate=GPIO_AF14_LTDC;
	HAL_GPIO_Init(GPIOI,&portInit);
	portInit.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	portInit.Mode=GPIO_MODE_AF_PP;
	portInit.Alternate=GPIO_AF14_LTDC;
	HAL_GPIO_Init(GPIOJ,&portInit);
	portInit.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
	portInit.Mode=GPIO_MODE_AF_PP;
	portInit.Alternate=GPIO_AF14_LTDC;
	HAL_GPIO_Init(GPIOK,&portInit);
	portInit.Pin=GPIO_PIN_12;
	portInit.Mode=GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(GPIOI,&portInit);
	portInit.Pin=GPIO_PIN_3;
	portInit.Mode=GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(GPIOK,&portInit);
	/* LCD orajel beallitasa (9.6 MHz) */
	clkInit.PeriphClockSelection=RCC_PERIPHCLK_LTDC;
	clkInit.PLLSAI.PLLSAIN=192;
	clkInit.PLLSAI.PLLSAIR=5;
	clkInit.PLLSAIDivR=RCC_PLLSAIDIVR_4;
	HAL_RCCEx_PeriphCLKConfig(&clkInit);
	/* LCD konfiguralasa */
	lcdHandle.Init.HorizontalSync=HSYNC-1;
	lcdHandle.Init.VerticalSync=VSYNC-1;
	lcdHandle.Init.AccumulatedHBP=HSYNC+HBP-1;
	lcdHandle.Init.AccumulatedVBP=VSYNC+VBP-1;
	lcdHandle.Init.AccumulatedActiveH=HEIGHT+VSYNC+VBP-1;
	lcdHandle.Init.AccumulatedActiveW=WIDTH+HSYNC+HBP-1;
	lcdHandle.Init.TotalHeigh=HEIGHT+VSYNC+VBP+VFP-1;
	lcdHandle.Init.TotalWidth=WIDTH+HSYNC+HBP+HFP-1;
	lcdHandle.LayerCfg->ImageWidth=WIDTH;
	lcdHandle.LayerCfg->ImageHeight=HEIGHT;
	lcdHandle.Init.Backcolor.Blue=0;
	lcdHandle.Init.Backcolor.Green=0;
	lcdHandle.Init.Backcolor.Red=0;
	lcdHandle.Init.HSPolarity=LTDC_HSPOLARITY_AL;
	lcdHandle.Init.VSPolarity=LTDC_VSPOLARITY_AL;
	lcdHandle.Init.DEPolarity=LTDC_DEPOLARITY_AL;
	lcdHandle.Init.PCPolarity=LTDC_PCPOLARITY_IPC;
	lcdHandle.Instance=LTDC;
	HAL_LTDC_Init(&lcdHandle);
	/* kijelzo engedelyezese es hattervilagitas bekapcsolasa */
	HAL_GPIO_WritePin(GPIOI,GPIO_PIN_12,GPIO_PIN_SET);
	lcdBackLightOn();
	/* alapertekek */
	fbAddr=FRAME_BUFFER_ADDR;
	currentLayer=0;
}

void lcdBackLightOff(void){
	HAL_GPIO_WritePin(GPIOK,GPIO_PIN_3,GPIO_PIN_RESET);
}

void lcdBackLightOn(void){
	HAL_GPIO_WritePin(GPIOK,GPIO_PIN_3,GPIO_PIN_SET);
}

void lcdBackLightToggle(void){
	HAL_GPIO_TogglePin(GPIOK,GPIO_PIN_3);
}

void lcdInitLayers(void){
	LTDC_LayerCfgTypeDef layerConfig;
	/* hatter */
	layerConfig.WindowX0=0;
	layerConfig.WindowX1=WIDTH;
	layerConfig.WindowY0=0;
	layerConfig.WindowY1=HEIGHT;
	layerConfig.PixelFormat=LTDC_PIXEL_FORMAT_RGB565;
	layerConfig.FBStartAdress=FRAME_BUFFER_ADDR;
	layerConfig.Alpha=255;
	layerConfig.Alpha0=0;
	layerConfig.Backcolor.Blue=0;
	layerConfig.Backcolor.Green=0;
	layerConfig.Backcolor.Red=0;
	layerConfig.BlendingFactor1=LTDC_BLENDING_FACTOR1_PAxCA;
	layerConfig.BlendingFactor2=LTDC_BLENDING_FACTOR2_PAxCA;
	layerConfig.ImageWidth=WIDTH;
	layerConfig.ImageHeight=HEIGHT;
	HAL_LTDC_ConfigLayer(&lcdHandle,&layerConfig,0);
	/* eloter */
	layerConfig.WindowX0=0;
	layerConfig.WindowX1=WIDTH;
	layerConfig.WindowY0=0;
	layerConfig.WindowY1=HEIGHT;
	layerConfig.PixelFormat=LTDC_PIXEL_FORMAT_RGB565;
	layerConfig.FBStartAdress=FRAME_BUFFER_ADDR+FRAME_BUFFER_SIZE;
	layerConfig.Alpha=255;
	layerConfig.Alpha0=0;
	layerConfig.Backcolor.Blue=0;
	layerConfig.Backcolor.Green=0;
	layerConfig.Backcolor.Red=0;
	layerConfig.BlendingFactor1=LTDC_BLENDING_FACTOR1_PAxCA;
	layerConfig.BlendingFactor2=LTDC_BLENDING_FACTOR2_PAxCA;
	layerConfig.ImageWidth=WIDTH;
	layerConfig.ImageHeight=HEIGHT;
	HAL_LTDC_ConfigLayer(&lcdHandle,&layerConfig,1);
}

void lcdSetLayer1(void){
	fbAddr=FRAME_BUFFER_ADDR;
	currentLayer=0;
}

void lcdSetLayer2(void){
	fbAddr=FRAME_BUFFER_ADDR+FRAME_BUFFER_SIZE;
	currentLayer=1;
}

void lcdSetTransparency(uint8_t t){
	if(currentLayer==0)
		HAL_LTDC_SetAlpha(&lcdHandle,t,0);
	else
		HAL_LTDC_SetAlpha(&lcdHandle,t,1);
}

void lcdCloneLayer1ToLayer2(void){
	uint32_t i;
	for(i=0;i<FRAME_BUFFER_SIZE;i+=2)
		*((volatile uint16_t*)(FRAME_BUFFER_ADDR+FRAME_BUFFER_SIZE+i))=*((volatile uint16_t*)(FRAME_BUFFER_ADDR+i));
}

void lcdCloneLayer2ToLayer1(void){
	uint32_t i;
	for(i=0;i<FRAME_BUFFER_SIZE;i+=2)
		*((volatile uint16_t*)(FRAME_BUFFER_ADDR+i))=*((volatile uint16_t*)(FRAME_BUFFER_ADDR+FRAME_BUFFER_SIZE+i));
}

void lcdSwap(void){
	if(currentLayer==0){
		lcdSetLayer2();
		lcdSetTransparency(0);
	}
	else{
		lcdSetTransparency(255);
		lcdSetLayer1();
	}
}
