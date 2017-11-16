#include "board.h"
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_gpio.h"

void boardInit(void){
	/* errata */
	CHIP_Init();
	/* orajel 24 MHz */
	CMU_ClockDivSet(cmuClock_HF,cmuClkDiv_2);
	CMU_OscillatorEnable(cmuOsc_HFXO,true,true);
	CMU_ClockSelectSet(cmuClock_HF,cmuSelect_HFXO);
	CMU_ClockEnable(cmuClock_HFPER,true);
	CMU_ClockEnable(cmuClock_GPIO,true);
	/* LED-ek (PE2-3) */
	GPIO_PinModeSet(gpioPortE,2,gpioModePushPull,0);
	GPIO_PinModeSet(gpioPortE,3,gpioModePushPull,0);
	/* nyomogombok (PB9-10) */
	GPIO_PinModeSet(gpioPortB,9,gpioModeInput,0);
	GPIO_PinModeSet(gpioPortB,10,gpioModeInput,0);
}

void ledOn(uint8_t n){
	if(n<2) GPIO_PinOutSet(gpioPortE,n+2);
}

void ledOff(uint8_t n){
	if(n<2) GPIO_PinOutClear(gpioPortE,n+2);
}

void ledToggle(uint8_t n){
	if(n<2) GPIO_PinOutToggle(gpioPortE,n+2);
}

bool ledGet(uint8_t n){
	if(n>1) return false;
	return GPIO_PinOutGet(gpioPortE,n+2)?true:false;
}

bool pbGet(uint8_t n){
	if(n>1) return false;
	return GPIO_PinInGet(gpioPortB,n+9)?false:true;
}
