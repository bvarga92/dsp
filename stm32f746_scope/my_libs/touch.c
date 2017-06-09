#include "touch.h"

touchData_t touchData;
static I2C_HandleTypeDef i2cHandle;

static uint8_t touchReadReg(uint8_t addr, uint8_t reg){
	uint8_t temp;
	HAL_I2C_Master_Transmit(&i2cHandle,addr,&reg,1,1000);
	HAL_I2C_Master_Receive(&i2cHandle,addr,&temp,1,1000);
	return temp;
}

void touchInit(void){
	static uint8_t init_ok=0;
	GPIO_InitTypeDef portInit;
	RCC_PeriphCLKInitTypeDef clkInit;
	/* mar inicializaltak */
	if(init_ok!=0) return;
	/* GPIO beallitasa (PH7, PH8) */
	__GPIOH_CLK_ENABLE();
	__I2C3_CLK_ENABLE();
	portInit.Pin=GPIO_PIN_7;
	portInit.Mode=GPIO_MODE_AF_OD;
	portInit.Pull=GPIO_PULLUP;
	portInit.Speed=GPIO_SPEED_HIGH;
	portInit.Alternate=GPIO_AF4_I2C3;
	HAL_GPIO_Init(GPIOH,&portInit);
	portInit.Pin=GPIO_PIN_8;
	portInit.Alternate=GPIO_AF4_I2C3;
	HAL_GPIO_Init(GPIOH,&portInit);
	/* orajel beallitasa*/
	clkInit.PeriphClockSelection=RCC_PERIPHCLK_I2C3;
	clkInit.I2c3ClockSelection=RCC_I2C3CLKSOURCE_PCLK1;
	HAL_RCCEx_PeriphCLKConfig(&clkInit);
	/* I2C3 konfiguralasa (100 kbps) */
	i2cHandle.Instance=I2C3;
	i2cHandle.Init.Timing=0x40912732;
	i2cHandle.Init.OwnAddress1=0;
	i2cHandle.Init.AddressingMode=I2C_ADDRESSINGMODE_7BIT;
	i2cHandle.Init.DualAddressMode=I2C_DUALADDRESS_DISABLE;
	i2cHandle.Init.OwnAddress2=0;
	i2cHandle.Init.GeneralCallMode=I2C_GENERALCALL_DISABLE;
	i2cHandle.Init.NoStretchMode=I2C_NOSTRETCH_DISABLE;
	HAL_I2C_Init(&i2cHandle);
	HAL_I2CEx_ConfigAnalogFilter(&i2cHandle,I2C_ANALOGFILTER_ENABLE);
	HAL_Delay(200);
	/* kiolvassuk az ID-t */
	if(touchReadReg(0x70,0xA8)==0x51) init_ok=1;
	/* default ertekek */
	touchData.cnt=0;
	touchData.status=TOUCH_RELEASED;
}

void touchRead(void){
	uint8_t temp, i;
	touchData.status=TOUCH_RELEASED;
	touchData.cnt=0;
	temp=touchReadReg(0x70,0x02)&0x0F;
	if((temp>0)&&(temp<=5)){
		touchData.status=TOUCH_PRESSED;
		touchData.cnt=temp;
		for(i=0;i<temp;i++){
			touchData.p[i].y=touchReadReg(0x70,i*6+4);
			touchData.p[i].y|=(touchReadReg(0x70,i*6+3)&0x0F)<<8;
			if(touchData.p[i].y>271) touchData.p[i].y=271;
			touchData.p[i].x=touchReadReg(0x70,i*6+6);
			touchData.p[i].x|=(touchReadReg(0x70,i*6+5)&0x0F)<<8;
			if(touchData.p[i].x>479) touchData.p[i].x=479;
		}
	}
}
