#include "audio.h"

/* codecet kezelo fuggvenyek prototipusai */
static void codecWriteReg(uint16_t reg, uint16_t data);
static uint16_t codecReadReg(uint16_t reg);
static void codecInit(uint16_t inputDevice, uint16_t outputDevice, uint8_t volume, uint32_t samplingRate);
static void codecReset(void);
static void codecSoftMute(void);
static void codecSoftUnmute(void);
static void codecSetVolume(uint8_t volume);


/* globalis valtozok */
static I2C_HandleTypeDef i2cHandle;
static uint8_t outputEnabled=0, inputEnabled=0;
static SAI_HandleTypeDef saiHandleOut, saiHandleIn;
volatile uint8_t inTransferHalfComplete=0;
volatile uint8_t inTransferComplete=0;
volatile uint8_t outTransferHalfComplete=0;
volatile uint8_t outTransferComplete=0;


static void codecWriteReg(uint16_t reg, uint16_t data){
	uint8_t temp[2];
	temp[0]=(uint8_t)(data>>8);
	temp[1]=(uint8_t)(data&0x00FF);
	HAL_I2C_Mem_Write(&i2cHandle,0x34,reg,I2C_MEMADD_SIZE_16BIT,temp,2,1000);
}

static uint16_t codecReadReg(uint16_t reg){
	uint8_t temp[2];
	HAL_I2C_Mem_Read(&i2cHandle,0x34,reg,I2C_MEMADD_SIZE_16BIT,temp,2,1000);
	return (uint16_t)((temp[0]<<8)|temp[1]);
}

static void codecInit(uint16_t inputDevice, uint16_t outputDevice, uint8_t volume, uint32_t samplingRate){
	GPIO_InitTypeDef portInit;
	/* I2C inizializalasa */
	if(HAL_I2C_GetState(&i2cHandle)==HAL_I2C_STATE_RESET){
		__GPIOH_CLK_ENABLE();
		portInit.Pin=GPIO_PIN_7|GPIO_PIN_8;
		portInit.Mode=GPIO_MODE_AF_OD;
		portInit.Pull=GPIO_NOPULL;
		portInit.Speed=GPIO_SPEED_FAST;
		portInit.Alternate=GPIO_AF4_I2C3;
		HAL_GPIO_Init(GPIOH,&portInit);
		__I2C3_CLK_ENABLE();
		__I2C3_FORCE_RESET();
		__I2C3_RELEASE_RESET();
		i2cHandle.Instance=I2C3;
		i2cHandle.Init.Timing=0x40912732;
		i2cHandle.Init.OwnAddress1=0;
		i2cHandle.Init.AddressingMode=I2C_ADDRESSINGMODE_7BIT;
		i2cHandle.Init.DualAddressMode=I2C_DUALADDRESS_DISABLE;
		i2cHandle.Init.OwnAddress2=0;
		i2cHandle.Init.GeneralCallMode=I2C_GENERALCALL_DISABLE;
		i2cHandle.Init.NoStretchMode=I2C_NOSTRETCH_DISABLE;
		HAL_I2C_Init(&i2cHandle);
		HAL_NVIC_SetPriority(I2C3_EV_IRQn,5,0);
		HAL_NVIC_EnableIRQ(I2C3_EV_IRQn);
		HAL_NVIC_SetPriority(I2C3_ER_IRQn,5,0);
		HAL_NVIC_EnableIRQ(I2C3_ER_IRQn);
	}
	/* bekapcsolas */
	codecReset();
	if(codecReadReg(0x0000)!=0x8994) while(1) ;
	codecWriteReg(0x0102,0x0003); //init patch
	codecWriteReg(0x0817,0x0000); //init patch
	codecWriteReg(0x0102,0x0000); //init patch
	codecWriteReg(0x0039,0x006C); //pop-mentes indulas: startup bias, vmid ramp: soft fast
	codecWriteReg(0x0001,0x0003); //bias, vmid
	HAL_Delay(50);
	/* kimenet */
	if(outputDevice){
		outputEnabled=1;
		switch(outputDevice){
			case OUTPUT_DEVICE_SPEAKER:
				codecWriteReg(0x0005,0x0C0C); //DAC2 be, DAC1 ki
				codecWriteReg(0x0601,0x0000); //AIF1 slot 0L -//-> DAC1L
				codecWriteReg(0x0602,0x0000); //AIF1 slot 0R -//-> DAC1R
				codecWriteReg(0x0604,0x0002); //AIF1 slot 1L --> DAC2L
				codecWriteReg(0x0605,0x0002); //AIF1 slot 1R --> DAC2R
				break;
			case OUTPUT_DEVICE_HEADPHONE:
				codecWriteReg(0x0005,0x0303); //DAC1 be, DAC2 ki
				codecWriteReg(0x0601,0x0001); //AIF1 slot 0L --> DAC1L
				codecWriteReg(0x0602,0x0001); //AIF1 slot 0R --> DAC1R
				codecWriteReg(0x0604,0x0000); //AIF1 slot 1L -//-> DAC2L
				codecWriteReg(0x0605,0x0000); //AIF1 slot 1R -//-> DAC2R
				break;
			case OUTPUT_DEVICE_BOTH:
				codecWriteReg(0x0005,0x0F0F); //DAC1 es DAC2 be
				codecWriteReg(0x0601,0x0001); //AIF1 slot 0L --> DAC1L
				codecWriteReg(0x0602,0x0001); //AIF1 slot 0R --> DAC1R
				codecWriteReg(0x0604,0x0002); //AIF1 slot 1L --> DAC2L
				codecWriteReg(0x0605,0x0002); //AIF1 slot 1R --> DAC2R
				break;
			default:
				codecWriteReg(0x0005,0x0303); //DAC1 be, DAC2 ki
				codecWriteReg(0x0601,0x0001); //AIF1 slot 0L --> DAC1L
				codecWriteReg(0x0602,0x0001); //AIF1 slot 0R --> DAC1R
				codecWriteReg(0x0604,0x0000); //AIF1 slot 1L -//-> DAC2L
				codecWriteReg(0x0605,0x0000); //AIF1 slot 1R -//-> DAC2R
				break;
		}
	}
	else{
		outputEnabled=0;
	}
	/* bemenet */
	if(inputDevice){
		inputEnabled=1;
		switch(inputDevice){
			case INPUT_DEVICE_DIGITAL_MICROPHONE_2:
				codecWriteReg(0x0004,0x0C30); //DMIC2 be (AIF1 slot 1)
				codecWriteReg(0x0450,0x00DB); //Dynamic Range Control, anti-clip, quick-release, signal activity detect
				codecWriteReg(0x0002,0x6000); //thermal shutdown be
				codecWriteReg(0x0608,0x0002); //AIF1 slot 1L --> DMIC2L
				codecWriteReg(0x0609,0x0002); //AIF1 slot 1R --> DMIC2R
				codecWriteReg(0x0700,0x000E); //GPIO1 = AIF1 DRC2 signal detect
				break;
			case INPUT_DEVICE_INPUT_LINE_1:
				codecWriteReg(0x0028,0x0011); //invertalo bemenetek
				codecWriteReg(0x0029,0x0035); //bal  +30 dB
				codecWriteReg(0x002A,0x0035); //jobb +30 dB
				codecWriteReg(0x0004,0x0303); //ADC be (AIF1 slot 0)
				codecWriteReg(0x0440,0x00DB); //Dynamic Range Control, anti-clip, quick-release, signal activity detect
				codecWriteReg(0x0002,0x6350); //thermal shutdown be, PGA be
				codecWriteReg(0x0606,0x0002); //AIF1 slot 0L --> ADCL
				codecWriteReg(0x0607,0x0002); //AIF1 slot 0R --> ADCR
				codecWriteReg(0x0700,0x000D); //GPIO1 = AIF1 DRC1 signal detect
				break;
			default:
				// tobbi nem tamogatott
				break;
		}
	}
	else{
		inputEnabled=0;
	}
	/* mintaveteli frekvencia, orajelek */
	switch(samplingRate){
		case AUDIO_FREQUENCY_8K: codecWriteReg(0x0210,0x0003); break; //8000 Hz
		case AUDIO_FREQUENCY_16K: codecWriteReg(0x0210,0x0033); break; //16000 Hz
		case AUDIO_FREQUENCY_32K: codecWriteReg(0x0210,0x0063); break; //32000 Hz
		case AUDIO_FREQUENCY_48K: codecWriteReg(0x0210,0x0083); break; //48000 Hz
		case AUDIO_FREQUENCY_96K: codecWriteReg(0x0210,0x00A3); break; //96000 Hz
		case AUDIO_FREQUENCY_11K: codecWriteReg(0x0210,0x0013); break; //11025 Hz
		case AUDIO_FREQUENCY_22K: codecWriteReg(0x0210,0x0043); break; //22050 Hz
		case AUDIO_FREQUENCY_44K: codecWriteReg(0x0210,0x0073); break; //44100 Hz
		default: codecWriteReg(0x0210,0x0083); break; //48000 Hz
	}
	codecWriteReg(0x0300,0x4010); //AIF1 formatum: 16 bit, I2S
	codecWriteReg(0x0302,0x0000); //AIF1 slave mode
	codecWriteReg(0x0208,0x000A); //AIF1 DSP orajel be
	codecWriteReg(0x0200,0x0001); //AIF1 orajel = MCLK1
	/* kimenet beallitasai */
	if(outputDevice){
		codecWriteReg(0x0003,0x0300); //SPKRVOL es SPKLVOL PGA be
		codecWriteReg(0x0022,0x0000); //SPKL hangero 0dB
		codecWriteReg(0x0023,0x0000); //SPKR hangero 0dB, class D
		codecWriteReg(0x0036,0x0300); //DAC2 --> SPK
		codecWriteReg(0x0001,0x3003); //SPKOUT be
		codecWriteReg(0x0051,0x0005); //"Class W" fulhallgato driver be
		codecWriteReg(0x0001,0x3303); //HPOUT1 be
		codecWriteReg(0x0060,0x0022); //HPOUT1 kozepso fokozat be
		codecWriteReg(0x004C,0x9F25); //toltespumpa be
		HAL_Delay(15);
		codecWriteReg(0x002D,0x0001); //DAC1L --> HPOUT1LVOL
		codecWriteReg(0x002E,0x0001); //DAC1R --> HPOUT1RVOL
		codecWriteReg(0x0003,0x0330); //MIXOUTL, MIXOUTR be
		codecWriteReg(0x0054,0x0033); //DC servo, trigger startup
		HAL_Delay(250);
		codecWriteReg(0x0060,0x00EE); //HPOUT1 kimeneti fokozat be, rovidzar ki
		codecWriteReg(0x0610,0x00C0); //DAC1L hangero
		codecWriteReg(0x0611,0x00C0); //DAC1R hangero
		codecWriteReg(0x0420,0x0000); //AIF1 slot 0 --> DAC1 unmute
		codecWriteReg(0x0612,0x00C0); //DAC2L hangero
		codecWriteReg(0x0613,0x00C0); //DAC2R hangero
		codecWriteReg(0x0422,0x0000); //AIF1 slot 1 --> DAC2 unmute
	}
	/* bemenet beallitasai */
	if(inputDevice){
		if((inputDevice==INPUT_DEVICE_DIGITAL_MICROPHONE_1)||(inputDevice==INPUT_DEVICE_DIGITAL_MICROPHONE_2)){
			codecWriteReg(0x0001,0x3313); //MIC bias 1 be
			codecWriteReg(0x0620,0x0002); //ADC tulmintavetelezes be
			codecWriteReg(0x0411,0x3800); //AIF1 ADC2 felulatereszto szuro be (voice mode 1)
		}
		else if((inputDevice==INPUT_DEVICE_INPUT_LINE_1)||(inputDevice==INPUT_DEVICE_INPUT_LINE_2)){
			codecWriteReg(0x0018,0x000B); //IN1L hangero 0dB
			codecWriteReg(0x001A,0x000B); //IN1R hangero 0dB
			codecWriteReg(0x0410,0x1800); //AIF1 ADC1 felulatereszto szuro be (hifi mode)
		}
	}
	codecSetVolume(volume);
}

static void codecReset(){
	codecWriteReg(0x0000,0x0000);
}

static void codecSoftMute(){
	if(!outputEnabled) return;
	codecWriteReg(0x0420,0x0200); //DAC1
	codecWriteReg(0x0422,0x0200); //DAC2
}

static void codecSoftUnmute(){
	if(!outputEnabled) return;
	codecWriteReg(0x0420,0x0000); //DAC1
	codecWriteReg(0x0422,0x0000); //DAC2
}

static void codecSetVolume(uint8_t volume){
	uint8_t convertedVolume=(volume>100)?100:((uint8_t)((volume*63)/100));
	if(outputEnabled){
		if(convertedVolume>0x3E){
			codecSoftUnmute();
			codecWriteReg(0x001C,0x003F|0x0140); //bal fulhallgato
			codecWriteReg(0x001D,0x003F|0x0140); //jobb fulhallgato
			codecWriteReg(0x0026,0x003F|0x0140); //bal hangszoro
			codecWriteReg(0x0027,0x003F|0x0140); //jobb hangszoro
		}
		else if(volume==0){
			codecSoftMute();
		}
		else{
			codecSoftUnmute();
			codecWriteReg(0x001C,convertedVolume|0x0140); //bal fulhallgato
			codecWriteReg(0x001D,convertedVolume|0x0140); //jobb fulhallgato
			codecWriteReg(0x0026,convertedVolume|0x0140); //bal hangszoro
			codecWriteReg(0x0027,convertedVolume|0x0140); //jobb hangszoro
		}
	}
	if(inputEnabled){
		convertedVolume=(volume>=100)?239:((uint8_t)((volume*240)/100));
		codecWriteReg(0x0400,convertedVolume|0x0100); //ADC1L
		codecWriteReg(0x0401,convertedVolume|0x0100); //ADC1R
		codecWriteReg(0x0404,convertedVolume|0x0100); //ADC2L
		codecWriteReg(0x0405,convertedVolume|0x0100); //ADC2R
	}
}

void audioInInit(uint16_t device, uint8_t volume, uint32_t samplingRate){
	RCC_PeriphCLKInitTypeDef rccInit;
	GPIO_InitTypeDef portInit;
	static DMA_HandleTypeDef dmaHandle;
	if((device!=INPUT_DEVICE_INPUT_LINE_1)&&(device!=INPUT_DEVICE_DIGITAL_MICROPHONE_2)) while(1) ;
	/* SAI letiltasa */
	saiHandleOut.Instance=SAI2_Block_A;
	saiHandleIn.Instance=SAI2_Block_B;
	__HAL_SAI_DISABLE(&saiHandleOut);
	__HAL_SAI_DISABLE(&saiHandleIn);
	HAL_SAI_DeInit(&saiHandleOut);
	HAL_SAI_DeInit(&saiHandleIn);
	/* SAI orajel beallitasa */
	HAL_RCCEx_GetPeriphCLKConfig(&rccInit);
	rccInit.PeriphClockSelection=RCC_PERIPHCLK_SAI2;
	rccInit.Sai2ClockSelection=RCC_SAI2CLKSOURCE_PLLI2S;
	if((samplingRate==AUDIO_FREQUENCY_11K)||(samplingRate==AUDIO_FREQUENCY_22K)||(samplingRate==AUDIO_FREQUENCY_44K)){ //SAI orajel=429/2/19=11.289 MHz
		rccInit.PLLI2S.PLLI2SP=8;
		rccInit.PLLI2S.PLLI2SN=429;
		rccInit.PLLI2S.PLLI2SQ=2;
		rccInit.PLLI2SDivQ=19;
		HAL_RCCEx_PeriphCLKConfig(&rccInit);
	}
	else{ //SAI orajel=344/7/1=49.142 MHz
		rccInit.PLLI2S.PLLI2SP=8;
		rccInit.PLLI2S.PLLI2SN=344;
		rccInit.PLLI2S.PLLI2SQ=7;
		rccInit.PLLI2SDivQ=1;
		HAL_RCCEx_PeriphCLKConfig(&rccInit);
	}
	__SAI2_CLK_ENABLE();
	/* GPIO beallitasa */
	__GPIOI_CLK_ENABLE();
	__GPIOD_CLK_ENABLE();
	__GPIOG_CLK_ENABLE();
	__GPIOH_CLK_ENABLE();
	portInit.Pin=GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
	portInit.Mode=GPIO_MODE_AF_PP;
	portInit.Pull=GPIO_NOPULL;
	portInit.Speed=GPIO_SPEED_HIGH;
	portInit.Alternate=GPIO_AF10_SAI2;
	HAL_GPIO_Init(GPIOI,&portInit);
	portInit.Pin=GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13;
	HAL_GPIO_Init(GPIOD,&portInit);
	portInit.Pin=GPIO_PIN_10;
	HAL_GPIO_Init(GPIOG,&portInit);
	portInit.Pin=GPIO_PIN_15;
	portInit.Mode=GPIO_MODE_INPUT;
	HAL_GPIO_Init(GPIOH,&portInit);
	/* DMA */
	__DMA2_CLK_ENABLE();
	dmaHandle.Instance=DMA2_Stream7;
	dmaHandle.Init.Channel=DMA_CHANNEL_0;
	dmaHandle.Init.Direction=DMA_PERIPH_TO_MEMORY;
	dmaHandle.Init.PeriphInc=DMA_PINC_DISABLE;
	dmaHandle.Init.MemInc=DMA_MINC_ENABLE;
	dmaHandle.Init.PeriphDataAlignment=DMA_PDATAALIGN_HALFWORD;
	dmaHandle.Init.MemDataAlignment=DMA_MDATAALIGN_HALFWORD;
	dmaHandle.Init.Mode=DMA_CIRCULAR;
	dmaHandle.Init.Priority=DMA_PRIORITY_HIGH;
	dmaHandle.Init.FIFOMode=DMA_FIFOMODE_DISABLE;
	dmaHandle.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;
	dmaHandle.Init.MemBurst=DMA_MBURST_SINGLE;
	dmaHandle.Init.PeriphBurst=DMA_PBURST_SINGLE;
	__HAL_LINKDMA(&saiHandleIn,hdmarx,dmaHandle);
	HAL_DMA_DeInit(&dmaHandle);
	HAL_DMA_Init(&dmaHandle);
	/* megszakitasok */
	HAL_NVIC_SetPriority(DMA2_Stream7_IRQn,6,0);
	HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);
	HAL_NVIC_SetPriority(EXTI15_10_IRQn,6,0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	/* SAI2 block A: master rx        SAI2 block B: slave rx, szinkron */
	saiHandleOut.Init.AudioFrequency=samplingRate;
	saiHandleOut.Init.AudioMode=SAI_MODEMASTER_RX;
	saiHandleOut.Init.NoDivider=SAI_MASTERDIVIDER_ENABLED;
	saiHandleOut.Init.Protocol=SAI_FREE_PROTOCOL;
	saiHandleOut.Init.DataSize=SAI_DATASIZE_16;
	saiHandleOut.Init.FirstBit=SAI_FIRSTBIT_MSB;
	saiHandleOut.Init.ClockStrobing=SAI_CLOCKSTROBING_RISINGEDGE;
	saiHandleOut.Init.Synchro=SAI_ASYNCHRONOUS;
	saiHandleOut.Init.OutputDrive=SAI_OUTPUTDRIVE_ENABLED;
	saiHandleOut.Init.FIFOThreshold=SAI_FIFOTHRESHOLD_1QF;
	saiHandleOut.FrameInit.FrameLength=64;
	saiHandleOut.FrameInit.ActiveFrameLength=32;
	saiHandleOut.FrameInit.FSDefinition=SAI_FS_CHANNEL_IDENTIFICATION;
	saiHandleOut.FrameInit.FSPolarity=SAI_FS_ACTIVE_LOW;
	saiHandleOut.FrameInit.FSOffset=SAI_FS_BEFOREFIRSTBIT;
	saiHandleOut.SlotInit.FirstBitOffset=0;
	saiHandleOut.SlotInit.SlotSize=SAI_SLOTSIZE_DATASIZE;
	saiHandleOut.SlotInit.SlotNumber=4;
	saiHandleOut.SlotInit.SlotActive=(device==INPUT_DEVICE_DIGITAL_MICROPHONE_2)?(SAI_SLOTACTIVE_1|SAI_SLOTACTIVE_3):(SAI_SLOTACTIVE_0|SAI_SLOTACTIVE_2);
	HAL_SAI_Init(&saiHandleOut);
	saiHandleIn.Init.AudioFrequency=samplingRate;
	saiHandleIn.Init.AudioMode=SAI_MODESLAVE_RX;
	saiHandleIn.Init.NoDivider=SAI_MASTERDIVIDER_ENABLED;
	saiHandleIn.Init.Protocol=SAI_FREE_PROTOCOL;
	saiHandleIn.Init.DataSize=SAI_DATASIZE_16;
	saiHandleIn.Init.FirstBit=SAI_FIRSTBIT_MSB;
	saiHandleIn.Init.ClockStrobing=SAI_CLOCKSTROBING_RISINGEDGE;
	saiHandleIn.Init.Synchro=SAI_SYNCHRONOUS;
	saiHandleIn.Init.OutputDrive=SAI_OUTPUTDRIVE_DISABLED;
	saiHandleIn.Init.FIFOThreshold=SAI_FIFOTHRESHOLD_1QF;
	saiHandleIn.FrameInit.FrameLength=64;
	saiHandleIn.FrameInit.ActiveFrameLength=32;
	saiHandleIn.FrameInit.FSDefinition=SAI_FS_CHANNEL_IDENTIFICATION;
	saiHandleIn.FrameInit.FSPolarity=SAI_FS_ACTIVE_LOW;
	saiHandleIn.FrameInit.FSOffset=SAI_FS_BEFOREFIRSTBIT;
	saiHandleIn.SlotInit.FirstBitOffset=0;
	saiHandleIn.SlotInit.SlotSize=SAI_SLOTSIZE_DATASIZE;
	saiHandleIn.SlotInit.SlotNumber=4;
	saiHandleIn.SlotInit.SlotActive=(device==INPUT_DEVICE_DIGITAL_MICROPHONE_2)?(SAI_SLOTACTIVE_1|SAI_SLOTACTIVE_3):(SAI_SLOTACTIVE_0|SAI_SLOTACTIVE_2);
	HAL_SAI_Init(&saiHandleIn);
	__HAL_SAI_ENABLE(&saiHandleOut);
	__HAL_SAI_ENABLE(&saiHandleIn);
	/* codec inicializalasa */
	codecInit(device,0,volume,samplingRate);
}

void audioSetInputBuffer(uint16_t *buffer, uint32_t length){
	HAL_SAI_Receive_DMA(&saiHandleIn,(uint8_t*)buffer,length);
}

void audioOutInit(uint16_t device, uint8_t volume, uint32_t samplingRate){
	RCC_PeriphCLKInitTypeDef rccInit;
	GPIO_InitTypeDef portInit;
	static DMA_HandleTypeDef dmaHandle;
	if((device!=OUTPUT_DEVICE_HEADPHONE)&&(device!=OUTPUT_DEVICE_SPEAKER)&&(device!=OUTPUT_DEVICE_BOTH)&&(device!=OUTPUT_DEVICE_AUTO)) while(1) ;
	/* SAI letiltasa */
	saiHandleOut.Instance=SAI2_Block_A;
	__HAL_SAI_DISABLE(&saiHandleOut);
	HAL_SAI_DeInit(&saiHandleOut);
	/* SAI orajel beallitasa */
	HAL_RCCEx_GetPeriphCLKConfig(&rccInit);
	rccInit.PeriphClockSelection=RCC_PERIPHCLK_SAI2;
	rccInit.Sai2ClockSelection=RCC_SAI2CLKSOURCE_PLLI2S;
	if((samplingRate==AUDIO_FREQUENCY_11K)||(samplingRate==AUDIO_FREQUENCY_22K)||(samplingRate==AUDIO_FREQUENCY_44K)){ //SAI orajel=429/2/19=11.289 MHz
		rccInit.PLLI2S.PLLI2SP=8;
		rccInit.PLLI2S.PLLI2SN=429;
		rccInit.PLLI2S.PLLI2SQ=2;
		rccInit.PLLI2SDivQ=19;
		HAL_RCCEx_PeriphCLKConfig(&rccInit);
	}
	else{ //SAI orajel=344/7/1=49.142 MHz
		rccInit.PLLI2S.PLLI2SP=8;
		rccInit.PLLI2S.PLLI2SN=344;
		rccInit.PLLI2S.PLLI2SQ=7;
		rccInit.PLLI2SDivQ=1;
		HAL_RCCEx_PeriphCLKConfig(&rccInit);
	}
	__SAI2_CLK_ENABLE();
	/* GPIO beallitasa */
	__GPIOI_CLK_ENABLE();
	__GPIOD_CLK_ENABLE();
	portInit.Pin=GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
	portInit.Mode=GPIO_MODE_AF_PP;
	portInit.Pull=GPIO_NOPULL;
	portInit.Speed=GPIO_SPEED_HIGH;
	portInit.Alternate=GPIO_AF10_SAI2;
	HAL_GPIO_Init(GPIOI,&portInit);
	portInit.Pin=GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13;
	HAL_GPIO_Init(GPIOD,&portInit);
	/* DMA */
	__DMA2_CLK_ENABLE();
	dmaHandle.Instance=DMA2_Stream4;
	dmaHandle.Init.Channel=DMA_CHANNEL_3;
	dmaHandle.Init.Direction=DMA_MEMORY_TO_PERIPH;
	dmaHandle.Init.PeriphInc=DMA_PINC_DISABLE;
	dmaHandle.Init.MemInc=DMA_MINC_ENABLE;
	dmaHandle.Init.PeriphDataAlignment=DMA_PDATAALIGN_HALFWORD;
	dmaHandle.Init.MemDataAlignment=DMA_MDATAALIGN_HALFWORD;
	dmaHandle.Init.Mode=DMA_CIRCULAR;
	dmaHandle.Init.Priority=DMA_PRIORITY_HIGH;
	dmaHandle.Init.FIFOMode=DMA_FIFOMODE_ENABLE;
	dmaHandle.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;
	dmaHandle.Init.MemBurst=DMA_MBURST_SINGLE;
	dmaHandle.Init.PeriphBurst=DMA_PBURST_SINGLE;
	__HAL_LINKDMA(&saiHandleOut,hdmatx,dmaHandle);
	HAL_DMA_DeInit(&dmaHandle);
	HAL_DMA_Init(&dmaHandle);
	/* megszakitasok */
	HAL_NVIC_SetPriority(DMA2_Stream4_IRQn,5,0);
	HAL_NVIC_EnableIRQ(DMA2_Stream4_IRQn);
	/* SAI2 block A: master tx */
	saiHandleOut.Init.AudioFrequency=samplingRate;
	saiHandleOut.Init.AudioMode=SAI_MODEMASTER_TX;
	saiHandleOut.Init.NoDivider=SAI_MASTERDIVIDER_ENABLED;
	saiHandleOut.Init.Protocol=SAI_FREE_PROTOCOL;
	saiHandleOut.Init.DataSize=SAI_DATASIZE_16;
	saiHandleOut.Init.FirstBit=SAI_FIRSTBIT_MSB;
	saiHandleOut.Init.ClockStrobing=SAI_CLOCKSTROBING_RISINGEDGE;
	saiHandleOut.Init.Synchro=SAI_ASYNCHRONOUS;
	saiHandleOut.Init.OutputDrive=SAI_OUTPUTDRIVE_ENABLED;
	saiHandleOut.Init.FIFOThreshold=SAI_FIFOTHRESHOLD_1QF;
	saiHandleOut.FrameInit.FrameLength=64;
	saiHandleOut.FrameInit.ActiveFrameLength=32;
	saiHandleOut.FrameInit.FSDefinition=SAI_FS_CHANNEL_IDENTIFICATION;
	saiHandleOut.FrameInit.FSPolarity=SAI_FS_ACTIVE_LOW;
	saiHandleOut.FrameInit.FSOffset=SAI_FS_BEFOREFIRSTBIT;
	saiHandleOut.SlotInit.FirstBitOffset=0;
	saiHandleOut.SlotInit.SlotSize=SAI_SLOTSIZE_DATASIZE;
	saiHandleOut.SlotInit.SlotNumber=4;
	saiHandleOut.SlotInit.SlotActive=SAI_SLOTACTIVE_0|SAI_SLOTACTIVE_2;
	HAL_SAI_Init(&saiHandleOut);
	__HAL_SAI_ENABLE(&saiHandleOut);
	/* codec inicializalasa */
	codecInit(0,device,volume,samplingRate);
}

void audioSetOutputBuffer(uint16_t *buffer, uint32_t length){
	HAL_SAI_Transmit_DMA(&saiHandleOut,(uint8_t*)buffer,(length<0xFFFF)?length:0xFFFF);
}

void audioInOutInit(uint16_t deviceIn, uint16_t deviceOut, uint8_t volume, uint32_t samplingRate){
	RCC_PeriphCLKInitTypeDef rccInit;
	GPIO_InitTypeDef portInit;
	static DMA_HandleTypeDef dmaHandleRx, dmaHandleTx;
	if((deviceIn!=INPUT_DEVICE_DIGITAL_MICROPHONE_2)&&(deviceIn!=INPUT_DEVICE_INPUT_LINE_1)) while(1) ;
	/* SAI letiltasa */
	saiHandleOut.Instance=SAI2_Block_A;
	__HAL_SAI_DISABLE(&saiHandleOut);
	HAL_SAI_DeInit(&saiHandleOut);
	saiHandleIn.Instance=SAI2_Block_B;
	__HAL_SAI_DISABLE(&saiHandleIn);
	HAL_SAI_DeInit(&saiHandleIn);
	/* SAI orajel beallitasa */
	HAL_RCCEx_GetPeriphCLKConfig(&rccInit);
	rccInit.PeriphClockSelection=RCC_PERIPHCLK_SAI2;
	rccInit.Sai2ClockSelection=RCC_SAI2CLKSOURCE_PLLI2S;
	if((samplingRate==AUDIO_FREQUENCY_11K)||(samplingRate==AUDIO_FREQUENCY_22K)||(samplingRate==AUDIO_FREQUENCY_44K)){ //SAI orajel=429/2/19=11.289 MHz
		rccInit.PLLI2S.PLLI2SP=8;
		rccInit.PLLI2S.PLLI2SN=429;
		rccInit.PLLI2S.PLLI2SQ=2;
		rccInit.PLLI2SDivQ=19;
		HAL_RCCEx_PeriphCLKConfig(&rccInit);
	}
	else{ //SAI orajel=344/7/1=49.142 MHz
		rccInit.PLLI2S.PLLI2SP=8;
		rccInit.PLLI2S.PLLI2SN=344;
		rccInit.PLLI2S.PLLI2SQ=7;
		rccInit.PLLI2SDivQ=1;
		HAL_RCCEx_PeriphCLKConfig(&rccInit);
	}
	__SAI2_CLK_ENABLE();
	/* GPIO beallitasa */
	__GPIOI_CLK_ENABLE();
	__GPIOD_CLK_ENABLE();
	__GPIOG_CLK_ENABLE();
	__GPIOH_CLK_ENABLE();
	portInit.Pin=GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
	portInit.Mode=GPIO_MODE_AF_PP;
	portInit.Pull=GPIO_NOPULL;
	portInit.Speed=GPIO_SPEED_HIGH;
	portInit.Alternate=GPIO_AF10_SAI2;
	HAL_GPIO_Init(GPIOI,&portInit);
	portInit.Pin=GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13;
	HAL_GPIO_Init(GPIOD,&portInit);
	portInit.Pin=GPIO_PIN_10;
	HAL_GPIO_Init(GPIOG,&portInit);
	portInit.Pin=GPIO_PIN_15;
	portInit.Mode=GPIO_MODE_INPUT;
	HAL_GPIO_Init(GPIOH,&portInit);
	/* DMA */
	__DMA2_CLK_ENABLE();
	dmaHandleRx.Instance=DMA2_Stream7;
	dmaHandleRx.Init.Channel=DMA_CHANNEL_0;
	dmaHandleRx.Init.Direction=DMA_PERIPH_TO_MEMORY;
	dmaHandleRx.Init.PeriphInc=DMA_PINC_DISABLE;
	dmaHandleRx.Init.MemInc=DMA_MINC_ENABLE;
	dmaHandleRx.Init.PeriphDataAlignment=DMA_PDATAALIGN_HALFWORD;
	dmaHandleRx.Init.MemDataAlignment=DMA_MDATAALIGN_HALFWORD;
	dmaHandleRx.Init.Mode=DMA_CIRCULAR;
	dmaHandleRx.Init.Priority=DMA_PRIORITY_HIGH;
	dmaHandleRx.Init.FIFOMode=DMA_FIFOMODE_DISABLE;
	dmaHandleRx.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;
	dmaHandleRx.Init.MemBurst=DMA_MBURST_SINGLE;
	dmaHandleRx.Init.PeriphBurst=DMA_PBURST_SINGLE;
	__HAL_LINKDMA(&saiHandleIn,hdmarx,dmaHandleRx);
	HAL_DMA_DeInit(&dmaHandleRx);
	HAL_DMA_Init(&dmaHandleRx);
	dmaHandleTx.Instance=DMA2_Stream4;
	dmaHandleTx.Init.Channel=DMA_CHANNEL_3;
	dmaHandleTx.Init.Direction=DMA_MEMORY_TO_PERIPH;
	dmaHandleTx.Init.PeriphInc=DMA_PINC_DISABLE;
	dmaHandleTx.Init.MemInc=DMA_MINC_ENABLE;
	dmaHandleTx.Init.PeriphDataAlignment=DMA_PDATAALIGN_HALFWORD;
	dmaHandleTx.Init.MemDataAlignment=DMA_MDATAALIGN_HALFWORD;
	dmaHandleTx.Init.Mode=DMA_CIRCULAR;
	dmaHandleTx.Init.Priority=DMA_PRIORITY_HIGH;
	dmaHandleTx.Init.FIFOMode=DMA_FIFOMODE_ENABLE;
	dmaHandleTx.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;
	dmaHandleTx.Init.MemBurst=DMA_MBURST_SINGLE;
	dmaHandleTx.Init.PeriphBurst= DMA_PBURST_SINGLE;
	__HAL_LINKDMA(&saiHandleOut,hdmatx,dmaHandleTx);
	HAL_DMA_DeInit(&dmaHandleTx);
	HAL_DMA_Init(&dmaHandleTx);
	/* megszakitasok */
	HAL_NVIC_SetPriority(DMA2_Stream7_IRQn,6,0);
	HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);
	HAL_NVIC_SetPriority(EXTI15_10_IRQn,6,0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	HAL_NVIC_SetPriority(DMA2_Stream4_IRQn,5,0);
	HAL_NVIC_EnableIRQ(DMA2_Stream4_IRQn);
	/* SAI2 block A: master tx        SAI2 block B: slave rx, szinkron */
	saiHandleOut.Init.AudioFrequency=samplingRate;
	saiHandleOut.Init.AudioMode=SAI_MODEMASTER_TX;
	saiHandleOut.Init.NoDivider=SAI_MASTERDIVIDER_ENABLED;
	saiHandleOut.Init.Protocol=SAI_FREE_PROTOCOL;
	saiHandleOut.Init.DataSize=SAI_DATASIZE_16;
	saiHandleOut.Init.FirstBit=SAI_FIRSTBIT_MSB;
	saiHandleOut.Init.ClockStrobing=SAI_CLOCKSTROBING_RISINGEDGE;
	saiHandleOut.Init.Synchro=SAI_ASYNCHRONOUS;
	saiHandleOut.Init.OutputDrive=SAI_OUTPUTDRIVE_ENABLED;
	saiHandleOut.Init.FIFOThreshold=SAI_FIFOTHRESHOLD_1QF;
	saiHandleOut.FrameInit.FrameLength=64;
	saiHandleOut.FrameInit.ActiveFrameLength=32;
	saiHandleOut.FrameInit.FSDefinition=SAI_FS_CHANNEL_IDENTIFICATION;
	saiHandleOut.FrameInit.FSPolarity=SAI_FS_ACTIVE_LOW;
	saiHandleOut.FrameInit.FSOffset=SAI_FS_BEFOREFIRSTBIT;
	saiHandleOut.SlotInit.FirstBitOffset=0;
	saiHandleOut.SlotInit.SlotSize=SAI_SLOTSIZE_DATASIZE;
	saiHandleOut.SlotInit.SlotNumber=4;
	saiHandleOut.SlotInit.SlotActive=SAI_SLOTACTIVE_0|SAI_SLOTACTIVE_2;
	HAL_SAI_Init(&saiHandleOut);
  	saiHandleIn.Init.AudioFrequency=samplingRate;
	saiHandleIn.Init.AudioMode=SAI_MODESLAVE_RX;
	saiHandleIn.Init.NoDivider=SAI_MASTERDIVIDER_ENABLED;
	saiHandleIn.Init.Protocol=SAI_FREE_PROTOCOL;
	saiHandleIn.Init.DataSize=SAI_DATASIZE_16;
	saiHandleIn.Init.FirstBit=SAI_FIRSTBIT_MSB;
	saiHandleIn.Init.ClockStrobing=SAI_CLOCKSTROBING_RISINGEDGE;
	saiHandleIn.Init.Synchro=SAI_SYNCHRONOUS;
	saiHandleIn.Init.OutputDrive=SAI_OUTPUTDRIVE_DISABLED;
	saiHandleIn.Init.FIFOThreshold=SAI_FIFOTHRESHOLD_1QF;
	saiHandleIn.FrameInit.FrameLength=64;
	saiHandleIn.FrameInit.ActiveFrameLength=32;
	saiHandleIn.FrameInit.FSDefinition=SAI_FS_CHANNEL_IDENTIFICATION;
	saiHandleIn.FrameInit.FSPolarity=SAI_FS_ACTIVE_LOW;
	saiHandleIn.FrameInit.FSOffset=SAI_FS_BEFOREFIRSTBIT;
	saiHandleIn.SlotInit.FirstBitOffset=0;
	saiHandleIn.SlotInit.SlotSize=SAI_SLOTSIZE_DATASIZE;
	saiHandleIn.SlotInit.SlotNumber=4;
	saiHandleIn.SlotInit.SlotActive=(deviceIn==INPUT_DEVICE_DIGITAL_MICROPHONE_2)?(SAI_SLOTACTIVE_1|SAI_SLOTACTIVE_3):(SAI_SLOTACTIVE_0|SAI_SLOTACTIVE_2);
	HAL_SAI_Init(&saiHandleIn);
	__HAL_SAI_ENABLE(&saiHandleOut);
	__HAL_SAI_ENABLE(&saiHandleIn);
	/* codec inicializalasa */
	codecInit(deviceIn,deviceOut,volume,samplingRate);
}

void HAL_SAI_TxHalfCpltCallback(SAI_HandleTypeDef *hsai){outTransferHalfComplete=1;}
void HAL_SAI_TxCpltCallback(SAI_HandleTypeDef *hsai){outTransferComplete=1;}
void HAL_SAI_RxHalfCpltCallback(SAI_HandleTypeDef *hsai){inTransferHalfComplete=1;}
void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef *hsai){inTransferComplete=1;}
void HAL_SAI_ErrorCallback(SAI_HandleTypeDef *hsai){}

void DMA2_Stream4_IRQHandler(void){HAL_DMA_IRQHandler(saiHandleOut.hdmatx);}
void DMA2_Stream7_IRQHandler(void){HAL_DMA_IRQHandler(saiHandleIn.hdmarx);}
