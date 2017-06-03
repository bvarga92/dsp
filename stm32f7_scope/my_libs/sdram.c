#include "sdram.h"

static SDRAM_HandleTypeDef sdramHandle;

uint8_t sdramInit(void){
	static uint8_t init_ok=0;
	GPIO_InitTypeDef portInit;
	FMC_SDRAM_TimingTypeDef timing;
	FMC_SDRAM_CommandTypeDef sdramCommand;
	volatile uint32_t modereg=0;
	uint16_t temp1, temp2;
	/* mar inicializaltak */
	if(init_ok!=0) return 0;
	/* GPIO beallitasa */
	__FMC_CLK_ENABLE();
	__GPIOC_CLK_ENABLE();
	__GPIOD_CLK_ENABLE();
	__GPIOE_CLK_ENABLE();
	__GPIOF_CLK_ENABLE();
	__GPIOG_CLK_ENABLE();
	__GPIOH_CLK_ENABLE();
	portInit.Mode=GPIO_MODE_AF_PP;
	portInit.Pull=GPIO_PULLUP;
	portInit.Speed=GPIO_SPEED_FAST;
	portInit.Alternate=GPIO_AF12_FMC;
	portInit.Pin=GPIO_PIN_3;
	HAL_GPIO_Init(GPIOC,&portInit);
	portInit.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_14|GPIO_PIN_15;
	HAL_GPIO_Init(GPIOD,&portInit);
	portInit.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	HAL_GPIO_Init(GPIOE,&portInit);
	portInit.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	HAL_GPIO_Init(GPIOF,&portInit);
	portInit.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_8|GPIO_PIN_15;
	HAL_GPIO_Init(GPIOG,&portInit);
	portInit.Pin=GPIO_PIN_3|GPIO_PIN_5;
	HAL_GPIO_Init(GPIOH,&portInit);
	/* beallitasok 100 MHz memoria orajelhez */
	timing.LoadToActiveDelay=2;
	timing.ExitSelfRefreshDelay=7;
	timing.SelfRefreshTime=4;
	timing.RowCycleDelay=7;
	timing.WriteRecoveryTime=2;
	timing.RPDelay=2;
	timing.RCDDelay=2;
	sdramHandle.Instance=FMC_SDRAM_DEVICE;
	sdramHandle.Init.SDBank=FMC_SDRAM_BANK1;
	sdramHandle.Init.ColumnBitsNumber=FMC_SDRAM_COLUMN_BITS_NUM_8;
	sdramHandle.Init.RowBitsNumber=FMC_SDRAM_ROW_BITS_NUM_12;
	sdramHandle.Init.MemoryDataWidth=FMC_SDRAM_MEM_BUS_WIDTH_16;
	sdramHandle.Init.InternalBankNumber=FMC_SDRAM_INTERN_BANKS_NUM_4;
	sdramHandle.Init.CASLatency=FMC_SDRAM_CAS_LATENCY_2;
	sdramHandle.Init.WriteProtection=FMC_SDRAM_WRITE_PROTECTION_DISABLE;
	sdramHandle.Init.SDClockPeriod=FMC_SDRAM_CLOCK_PERIOD_2;
	sdramHandle.Init.ReadBurst=FMC_SDRAM_RBURST_ENABLE;
	sdramHandle.Init.ReadPipeDelay=FMC_SDRAM_RPIPE_DELAY_0;
	HAL_SDRAM_Init(&sdramHandle,&timing);
	/* konfiguracios parancsok */
	sdramCommand.CommandMode=FMC_SDRAM_CMD_CLK_ENABLE; //clock configuration enable
	sdramCommand.CommandTarget=FMC_SDRAM_CMD_TARGET_BANK1;
	sdramCommand.AutoRefreshNumber=1;
	sdramCommand.ModeRegisterDefinition=0;
	HAL_SDRAM_SendCommand(&sdramHandle,&sdramCommand,0xFFFF);
	HAL_Delay(1);
	sdramCommand.CommandMode=FMC_SDRAM_CMD_PALL; //precharge all
	sdramCommand.CommandTarget=FMC_SDRAM_CMD_TARGET_BANK1;
	sdramCommand.AutoRefreshNumber=1;
	sdramCommand.ModeRegisterDefinition=0;
	HAL_SDRAM_SendCommand(&sdramHandle,&sdramCommand,0xFFFF);
	sdramCommand.CommandMode=FMC_SDRAM_CMD_AUTOREFRESH_MODE; //auto refresh
	sdramCommand.CommandTarget=FMC_SDRAM_CMD_TARGET_BANK1;
	sdramCommand.AutoRefreshNumber=8;
	sdramCommand.ModeRegisterDefinition=0;
	HAL_SDRAM_SendCommand(&sdramHandle,&sdramCommand,0xFFFF);
	modereg=(uint32_t)(SDRAM_MODEREG_BURST_LENGTH_1|SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL|SDRAM_MODEREG_CAS_LATENCY_2|SDRAM_MODEREG_OPERATING_MODE_STANDARD|SDRAM_MODEREG_WRITEBURST_MODE_SINGLE);
	sdramCommand.CommandMode=FMC_SDRAM_CMD_LOAD_MODE; //mode register
	sdramCommand.CommandTarget=FMC_SDRAM_CMD_TARGET_BANK1;
	sdramCommand.AutoRefreshNumber=1;
	sdramCommand.ModeRegisterDefinition=modereg;
	HAL_SDRAM_SendCommand(&sdramHandle,&sdramCommand,0xFFFF);
	HAL_SDRAM_ProgramRefreshRate(&sdramHandle,0x0603);
	/* probairas a 0 cimre */
	temp1=sdramRead16(0x00);
	sdramWrite16(0x00,0x1234);
	temp2=sdramRead16(0x00);
	sdramWrite16(0x00,temp1);
	if(temp2==0x1234){ //ok
		init_ok=1;
		return 0;
	}
	else //hiba
		return 1;
}

void sdramWrite8(uint32_t addr, uint8_t data){
	*((uint8_t*)(SDRAM_BASE_ADDR+addr))=data;
}

uint8_t sdramRead8(uint32_t addr){
	uint8_t data;
	data=*((volatile uint8_t*)(SDRAM_BASE_ADDR+addr));
	return data;
}


void sdramWrite16(uint32_t addr, uint16_t data){
	*((uint16_t*)(SDRAM_BASE_ADDR+addr))=data;
}

uint16_t sdramRead16(uint32_t addr){
	uint16_t data;
	data=*((volatile uint16_t*)(SDRAM_BASE_ADDR+addr));
	return data;
}

void sdramWrite32(uint32_t addr, uint32_t data){
	*((uint32_t*)(SDRAM_BASE_ADDR+addr))=data;
}

uint32_t sdramRead32(uint32_t addr){
	uint32_t data;
	data=*((volatile uint32_t*)(SDRAM_BASE_ADDR+addr));
	return data;
}

void sdramWriteBuffer32(uint32_t* buffer, uint32_t startAddr, uint32_t len){
	HAL_SDRAM_Write_32b(&sdramHandle,(uint32_t*)(startAddr+SDRAM_BASE_ADDR),buffer,len);
}

void sdramReadBuffer32(uint32_t* buffer, uint32_t startAddr, uint32_t len){
	HAL_SDRAM_Read_32b(&sdramHandle,(uint32_t*)(startAddr+SDRAM_BASE_ADDR),buffer,len);
}

void sdramWriteBuffer32DMA(uint32_t* buffer, uint32_t startAddr, uint32_t len){
	HAL_SDRAM_Write_DMA(&sdramHandle,(uint32_t*)(startAddr+SDRAM_BASE_ADDR),buffer,len);
}
