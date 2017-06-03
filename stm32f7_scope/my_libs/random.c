#include "random.h"

static RNG_HandleTypeDef rngHandle;

void randomInit(void){
	__RNG_CLK_ENABLE();
	rngHandle.Instance=RNG;
	HAL_RNG_Init(&rngHandle);
}

uint32_t randomGet(void){
	uint32_t temp;
	if(rngHandle.State==HAL_RNG_STATE_RESET) randomInit();
	HAL_RNG_GenerateRandomNumber(&rngHandle,&temp);
	return temp;
}
