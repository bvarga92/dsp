#include "system.h"

void configMPU(void){
	MPU_Region_InitTypeDef MPU_InitStruct;
	HAL_MPU_Disable();
	MPU_InitStruct.Enable=MPU_REGION_ENABLE;
	MPU_InitStruct.BaseAddress=0x20010000;
	MPU_InitStruct.Size=MPU_REGION_SIZE_256KB;
	MPU_InitStruct.AccessPermission=MPU_REGION_FULL_ACCESS;
	MPU_InitStruct.IsBufferable=MPU_ACCESS_NOT_BUFFERABLE;
	MPU_InitStruct.IsCacheable=MPU_ACCESS_CACHEABLE;
	MPU_InitStruct.IsShareable=MPU_ACCESS_NOT_SHAREABLE;
	MPU_InitStruct.Number=MPU_REGION_NUMBER0;
	MPU_InitStruct.TypeExtField=MPU_TEX_LEVEL0;
	MPU_InitStruct.SubRegionDisable=0x00;
	MPU_InitStruct.DisableExec=MPU_INSTRUCTION_ACCESS_ENABLE;
	HAL_MPU_ConfigRegion(&MPU_InitStruct);
	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

void enableCache(void){
	/* utasitas cache es branch prediction */
	SCB_InvalidateICache();
	SCB->CCR|=(1<<18);
	__DSB();
	SCB_EnableICache();
	/* adat cache */
	SCB_InvalidateDCache();
	SCB_EnableDCache();
}

void configSystemClock(void){
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	/* PLL beallitasa, forras a HSE, 200 MHz */
	RCC_OscInitStruct.OscillatorType=RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState=RCC_HSE_ON;
	RCC_OscInitStruct.HSIState=RCC_HSI_OFF;
	RCC_OscInitStruct.PLL.PLLState=RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource=RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM=25;
	RCC_OscInitStruct.PLL.PLLN=400;
	RCC_OscInitStruct.PLL.PLLP=RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ=8;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);
	HAL_PWREx_EnableOverDrive();
	/* buszorajelek a PLL-rol */
	RCC_ClkInitStruct.ClockType=(RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider=RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider=RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider=RCC_HCLK_DIV2;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct,FLASH_LATENCY_5);
}

void ledInit(void){
	GPIO_InitTypeDef portInit;
	__GPIOI_CLK_ENABLE();
	portInit.Pin=GPIO_PIN_1;
	portInit.Mode=GPIO_MODE_OUTPUT_PP;
	portInit.Pull=GPIO_NOPULL;
	portInit.Speed=GPIO_SPEED_LOW;
	HAL_GPIO_Init(GPIOI,&portInit);
}

void ledOn(void){
	HAL_GPIO_WritePin(GPIOI,GPIO_PIN_1,GPIO_PIN_SET);
}

void ledOff(void){
	HAL_GPIO_WritePin(GPIOI,GPIO_PIN_1,GPIO_PIN_RESET);
}

void ledToggle(void){
	HAL_GPIO_TogglePin(GPIOI,GPIO_PIN_1);
}

void buttonInit(void){
	GPIO_InitTypeDef portInit;
	__GPIOI_CLK_ENABLE();
	portInit.Pin=GPIO_PIN_11;
	portInit.Mode=GPIO_MODE_INPUT;
	portInit.Pull=GPIO_NOPULL;
	portInit.Speed=GPIO_SPEED_LOW;
	HAL_GPIO_Init(GPIOI,&portInit);
}

uint8_t buttonRead(void){
	if(HAL_GPIO_ReadPin(GPIOI,GPIO_PIN_11))
		return 1;
	else
		return 0;
}
