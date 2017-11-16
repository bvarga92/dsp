#include "lcd.h"
#include "em_device.h"
#include "em_cmu.h"
#include "em_lcd.h"

static const uint8_t alphaPins[7][14][2]={
	{{13,1},{14,1},{14,5},{14,7},{13,7},{13,3},{13,4},{13,2},{14,3},{14,2},{14,4},{14,6},{13,5},{13,6}},
	{{15,1},{16,1},{16,5},{16,7},{15,7},{15,3},{15,4},{15,2},{16,3},{16,2},{16,4},{16,6},{15,5},{15,6}},
	{{17,1},{18,1},{18,5},{18,7},{17,7},{17,3},{17,4},{17,2},{18,3},{18,2},{18,4},{18,6},{17,5},{17,6}},
	{{19,1},{28,1},{28,5},{28,7},{19,7},{19,3},{19,4},{19,2},{28,3},{28,2},{28,4},{28,6},{19,5},{19,6}},
	{{29,0},{30,1},{30,5},{30,7},{29,6},{29,2},{29,3},{29,1},{30,3},{30,2},{30,4},{30,6},{29,4},{29,5}},
	{{31,0},{32,1},{32,5},{32,7},{31,6},{31,2},{31,3},{31,1},{32,3},{32,2},{32,4},{32,6},{31,4},{31,5}},
	{{33,1},{34,1},{34,5},{34,7},{33,7},{33,3},{33,4},{33,2},{34,3},{34,2},{34,4},{34,6},{33,5},{33,6}}
};

static const uint16_t alphaChars[]={
	0x0000, // space
	0x1100, // !
	0x0280, // "
	0x0000, // #
	0x0000, // $
	0x0602, // %
	0x0000, // &
	0x0020, // '
	0x0039, // (
	0x000F, // )
	0x0000, // *
	0x1540, // +
	0x2000, // ,
	0x0440, // -
	0x1000, // .
	0x2200, // /
	0x003F, // 0
	0x0006, // 1
	0x045B, // 2
	0x044F, // 3
	0x0466, // 4
	0x046D, // 5
	0x047D, // 6
	0x0007, // 7
	0x047F, // 8
	0x046F, // 9
	0x0000, // :
	0x0000, // ;
	0x0A00, // <
	0x0000, // =
	0x2080, // >
	0x0000, // ?
	0xFFFF, // @
	0x0477, // A
	0x0A79, // B
	0x0039, // C
	0x20B0, // D
	0x0079, // E
	0x0071, // F
	0x047D, // G
	0x0476, // H
	0x0006, // I
	0x000E, // J
	0x0A70, // K
	0x0038, // L
	0x02B6, // M
	0x08B6, // N
	0x003F, // O
	0x0473, // P
	0x083F, // Q
	0x0C73, // R
	0x046D, // S
	0x1101, // T
	0x003E, // U
	0x2230, // V
	0x2836, // W
	0x2A80, // X
	0x046E, // Y
	0x2209, // Z
	0x0039, // [
	0x0880, // backslash
	0x000F, // ]
	0x0001, // ^
	0x0008, // _
	0x0100, // `
	0x1058, // a
	0x047C, // b
	0x0058, // c
	0x045E, // d
	0x2058, // e
	0x0471, // f
	0x0C0C, // g
	0x0474, // h
	0x0004, // i
	0x000E, // j
	0x0C70, // k
	0x0038, // l
	0x1454, // m
	0x0454, // n
	0x045C, // o
	0x0473, // p
	0x0467, // q
	0x0450, // r
	0x0C08, // s
	0x0078, // t
	0x001C, // u
	0x2010, // v
	0x2814, // w
	0x2A80, // x
	0x080C, // y
	0x2048  // z
};

void lcdInit(bool boost){
	static LCD_Init_TypeDef lcdInitStruct;
	uint8_t i;
	/* orajel */
	CMU_ClockEnable(cmuClock_CORELE,true);
	CMU_OscillatorEnable(cmuSelect_LFRCO,true,true);
	CMU_ClockSelectSet(cmuClock_LFA,cmuSelect_LFRCO);
	CMU_ClockDivSet(cmuClock_LCDpre,cmuClkDiv_64);
	CMU_LCDClkFDIVSet(cmuClkDiv_1);
	CMU_ClockEnable(cmuClock_LCD,true);
	/* megszakitasok ki */
	LCD_IntDisable(0xFFFFFFFF);
	/* LCD vezerlo inicializalasa */
	lcdInitStruct.bias=lcdBiasOneFourth;
	lcdInitStruct.contrast=lcdConConfVLCD;
	lcdInitStruct.enable=true;
	lcdInitStruct.mux=lcdMuxOctaplex;
	lcdInitStruct.vlcd=lcdVLCDSelVDD;
	lcdInitStruct.wave=lcdWaveLowPower;
	LCD_Init(&lcdInitStruct);
	/* minden szegmens engedelyezve */
	LCD_SegmentRangeEnable(lcdSegment12_15,true);
	LCD_SegmentRangeEnable(lcdSegment16_19,true);
	LCD_SegmentRangeEnable(lcdSegment28_31,true);
	LCD_SegmentRangeEnable(lcdSegment32_35,true);
	LCD_SegmentRangeEnable(lcdSegment36_39,true);
	/* boost */
	if(boost){
		LCD_VBoostSet(lcdVBoostLevel3);
		LCD_VLCDSelect(lcdVLCDSelVExtBoost);
		CMU->LCDCTRL|=CMU_LCDCTRL_VBOOSTEN;
	}
	/* minden szegmens ki */
	for(i=0;i<8;i++){
		LCD_SegmentSetLow(i,0xF00FF000,0x00000000);
		LCD_SegmentSetHigh(i,0x000000FF,0x00000000);
	}
	LCD_SyncBusyDelay(0xFFFFFFFF);
}

void lcdSymbol(lcdSymbol_t s, bool on){
	uint8_t com, bit;
	switch(s){
		case LCD_SYM_GECKO:    com=1; bit=12; break;
		case LCD_SYM_ANT:      com=0; bit=32; break;
		case LCD_SYM_LOCK0:    com=3; bit=39; break;
		case LCD_SYM_LOCK1:    com=2; bit=12; break;
		case LCD_SYM_EFM32:    com=0; bit=28; break;
		case LCD_SYM_MINUS:    com=3; bit=12; break;
		case LCD_SYM_COLON_L1: com=4; bit=12; break;
		case LCD_SYM_COLON_L2: com=0; bit=30; break;
		case LCD_SYM_COLON_U:  com=5; bit=39; break;
		case LCD_SYM_DEGC:     com=0; bit=34; break;
		case LCD_SYM_DEGF:     com=0; bit=35; break;
		case LCD_SYM_DP_L1:    com=7; bit=12; break;
		case LCD_SYM_DP_L2:    com=5; bit=12; break;
		case LCD_SYM_DP_L3:    com=6; bit=12; break;
		case LCD_SYM_DP_L4:    com=7; bit=29; break;
		case LCD_SYM_DP_L5:    com=7; bit=31; break;
		case LCD_SYM_DP_U:     com=4; bit=39; break;
	}
	LCD_SegmentSet(com,bit,on);
}

void lcdEnergyMode(uint8_t em, bool on){
	switch(em){
		case 0: LCD_SegmentSet(0,39,on); break;
		case 1: LCD_SegmentSet(1,39,on); break;
		case 2: LCD_SegmentSet(7,39,on); break;
		case 3: LCD_SegmentSet(2,39,on); break;
		case 4: LCD_SegmentSet(6,39,on); break;
	}
}

void lcdRingIndicator(uint8_t pattern){
	uint8_t i;
	for(i=0;i<8;i++) LCD_SegmentSet(0,19-i,(pattern&(1<<i))?true:false);
}

void lcdBattery(uint8_t level){
	if(level>4) return;
	LCD_SegmentSet(0,33,(0<level)?true:false);
	LCD_SegmentSet(0,37,(1<level)?true:false);
	LCD_SegmentSet(0,36,(2<level)?true:false);
	LCD_SegmentSet(0,38,(3<level)?true:false);
}

void lcdUpperNumberOff(void){
	uint8_t i;
	for(i=1;i<8;i++) LCD_SegmentSetHigh(i,0x00000078,0x00000000);
}

void lcdUpperNumberPrint(int16_t n){
	uint8_t digits[4], i, j, seg, com;
	bool negative;
	if((n<-999)||(n>9999)) return;
	if(n<0){
		n=-n;
		negative=true;
	}
	else{
		negative=false;
	}
	if(n<10){
		digits[0]=n;
		digits[1]=negative?10:11;
		digits[2]=17;
		digits[3]=17;
	}
	else if(n<100){
		digits[0]=n%10;
		digits[1]=n/10;
		digits[2]=negative?10:11;
		digits[3]=17;
	}
	else if(n<1000){
		digits[0]=n%10;
		digits[1]=(n/10)%10;
		digits[2]=(n/100)%10;
		digits[3]=negative?10:11;
	}
	else{
		digits[0]=n%10;
		digits[1]=(n/10)%10;
		digits[2]=(n/100)%10;
		digits[3]=n/1000;
	}
	LCD_SyncBusyDelay(0xFFFFFFFF);
	LCD_FreezeEnable(true);
	lcdUpperNumberOff();
	for(i=0;i<4;i++){
		switch(digits[i]){
			case  0: seg=0x3F; break; //0
			case  1: seg=0x06; break; //1
			case  2: seg=0x5B; break; //2
			case  3: seg=0x4F; break; //3
			case  4: seg=0x66; break; //4
			case  5: seg=0x6D; break; //5
			case  6: seg=0x7D; break; //6
			case  7: seg=0x07; break; //7
			case  8: seg=0x7F; break; //8
			case  9: seg=0x6F; break; //9
			case 10: seg=0x40; break; //-
			default: seg=0x00; break; //kikapcs
		}
		for(j=0;j<7;j++){
			if(seg&(1<<j)){
				switch(j){
					case 0: com=7; break;
					case 1: com=5; break;
					case 2: com=2; break;
					case 3: com=1; break;
					case 4: com=3; break;
					case 5: com=6; break;
					case 6: com=4; break;
				}
				LCD_SegmentSet(com,35+i,true);
			}
		}
	}
	LCD_FreezeEnable(false);
}

void lcdUpperNumberPrintHex(uint16_t n){
	uint8_t i, j, seg, com;
	LCD_SyncBusyDelay(0xFFFFFFFF);
	LCD_FreezeEnable(true);
	lcdUpperNumberOff();
	for(i=0;i<4;i++){
		switch((n>>(i<<2))&15){
			case  0: seg=0x3F; break; //0
			case  1: seg=0x06; break; //1
			case  2: seg=0x5B; break; //2
			case  3: seg=0x4F; break; //3
			case  4: seg=0x66; break; //4
			case  5: seg=0x6D; break; //5
			case  6: seg=0x7D; break; //6
			case  7: seg=0x07; break; //7
			case  8: seg=0x7F; break; //8
			case  9: seg=0x6F; break; //9
			case 10: seg=0x77; break; //A
			case 11: seg=0x7C; break; //b
			case 12: seg=0x39; break; //C
			case 13: seg=0x5E; break; //d
			case 14: seg=0x79; break; //E
			case 15: seg=0x71; break; //F
		}
		for(j=0;j<7;j++){
			if(seg&(1<<j)){
				switch(j){
					case 0: com=7; break;
					case 1: com=5; break;
					case 2: com=2; break;
					case 3: com=1; break;
					case 4: com=3; break;
					case 5: com=6; break;
					case 6: com=4; break;
				}
				LCD_SegmentSet(com,35+i,true);
			}
		}
	}
	LCD_FreezeEnable(false);
}

void lcdAlphaOff(void){
	LCD_SegmentSetLow(0,0xA0000000,0x00000000);
	LCD_SegmentSetLow(1,0xF00FE000,0x00000000);
	LCD_SegmentSetLow(2,0xF00FE000,0x00000000);
	LCD_SegmentSetLow(3,0xF00FE000,0x00000000);
	LCD_SegmentSetLow(4,0xF00FE000,0x00000000);
	LCD_SegmentSetLow(5,0xF00FE000,0x00000000);
	LCD_SegmentSetLow(6,0xF00FE000,0x00000000);
	LCD_SegmentSetLow(7,0x500FE000,0x00000000);
	LCD_SegmentSetHigh(1,0x00000007,0x00000000);
	LCD_SegmentSetHigh(2,0x00000007,0x00000000);
	LCD_SegmentSetHigh(3,0x00000007,0x00000000);
	LCD_SegmentSetHigh(4,0x00000007,0x00000000);
	LCD_SegmentSetHigh(5,0x00000007,0x00000000);
	LCD_SegmentSetHigh(6,0x00000007,0x00000000);
	LCD_SegmentSetHigh(7,0x00000007,0x00000000);
}

void lcdAlphaPrint(char *str){
	uint8_t length=0, digit, i;
	uint16_t seg;
	while(str[length]) length++;
	LCD_SyncBusyDelay(0xFFFFFFFF);
	LCD_FreezeEnable(true);
	lcdAlphaOff();
	for(digit=0;digit<7;digit++){
		seg=alphaChars[(digit<length)?(str[digit]-0x20):0];
		for(i=0;i<14;i++) LCD_SegmentSet(alphaPins[digit][i][1],alphaPins[digit][i][0],(seg&(1<<i))?true:false);
	}
	LCD_FreezeEnable(false);
}
