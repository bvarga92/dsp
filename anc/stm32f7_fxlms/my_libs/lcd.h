#ifndef _LCD_H_
#define _LCD_H_

	#include "stm32f7xx.h"
	#include "stm32f7xx_hal.h"
	#include "stm32f7xx_hal_cortex.h"
	#include "stm32f7xx_hal_ltdc.h"

	/* a frame buffer kezdocime es merete */
	#define FRAME_BUFFER_ADDR   SDRAM_BASE_ADDR
	#define FRAME_BUFFER_SIZE   ((uint32_t)(WIDTH*HEIGHT*2))

	/* a kijelzo merete */
	#define WIDTH               ((uint16_t)480)
	#define HEIGHT              ((uint16_t)272)

	/* makrok az idozitesekhez */
	#define HSYNC               ((uint16_t)41)
	#define HBP                 ((uint16_t)13)
	#define HFP                 ((uint16_t)32)
	#define VSYNC               ((uint16_t)10)
	#define VBP                 ((uint16_t)2)
	#define VFP                 ((uint16_t)2)


	/* Frame buffer cime, ide lehet rajzolni (sorfolytonos tarolas, 16 bites szinmelyseg --> 255 kB). */
	extern uint32_t fbAddr;

	/* melyik reteg aktiv (0/1) */
	extern uint8_t currentLayer;


	/* Inicializalja a kijelzot. */
	void lcdInit(void);

	/* Kikapcsolja a kijelzo hattervilagitasat. */
	void lcdBackLightOff(void);

	/* Bekapcsolja a kijelzo hattervilagitasat. */
	void lcdBackLightOn(void);

	/* Invertalja a kijelzo hattervilagitasat. */
	void lcdBackLightToggle(void);

	/* Mindket reteget feketere inicializalja. */
	void lcdInitLayers(void);

	/* 1-es reteg aktivalasa. */
	void lcdSetLayer1(void);

	/* 2-es reteg aktivalasa. */
	void lcdSetLayer2(void);

	/* Beallitja az aktiv reteg atlatszosagat (0: atlatszo, 255: tomor). */
	void lcdSetTransparency(uint8_t t);

	/* Az 1-es reteg tartalmat atmasolja a 2-es retegre. */
	void lcdCloneLayer1ToLayer2(void);

	/* A 2-es reteg tartalmat atmasolja az 1-es retegre. */
	void lcdCloneLayer2ToLayer1(void);

	/* Az eddig nem latszo reteget teszi aktivva. */
	void lcdSwap(void);

#endif
