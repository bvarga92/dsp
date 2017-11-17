#ifndef _LCD_H_
#define _LCD_H_

	#include "stdbool.h"
	#include "stdint.h"

	typedef enum{
		LCD_SYM_GECKO,
		LCD_SYM_ANT,
		LCD_SYM_LOCK0,
		LCD_SYM_LOCK1,
		LCD_SYM_EFM32,
		LCD_SYM_MINUS,
		LCD_SYM_COLON_L1,
		LCD_SYM_COLON_L2,
		LCD_SYM_COLON_U,
		LCD_SYM_DEGC,
		LCD_SYM_DEGF,
		LCD_SYM_DP_L1,
		LCD_SYM_DP_L2,
		LCD_SYM_DP_L3,
		LCD_SYM_DP_L4,
		LCD_SYM_DP_L5,
		LCD_SYM_DP_U
	} lcdSymbol_t;

	void lcdInit(bool boost);
	void lcdSymbol(lcdSymbol_t s, bool on);
	void lcdEnergyMode(uint8_t em, bool on);
	void lcdRingIndicator(uint8_t pattern);
	void lcdBattery(uint8_t level);
	void lcdUpperNumberOff(void);
	void lcdUpperNumberPrint(int16_t n);
	void lcdUpperNumberPrintHex(uint16_t n);
	void lcdAlphaOff(void);
	void lcdAlphaPrint(char *str);

#endif
