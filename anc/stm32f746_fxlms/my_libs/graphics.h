#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

	#include "lcd.h"
	#include "stm32f7xx_hal_dma2d.h"

	/* tipus kepek tarolasahoz */
	typedef struct{
		const uint16_t *data; //pointer a bitterkephez (sorfolytonos tarolas R5G6B5 formatumban)
		uint16_t w; //szelesseg
		uint16_t h; //magassag
	} image_t;


	/* nehany szin (R5G6B5 formatum) */
	#define COLOR_BLACK          0x0000
	#define COLOR_BLUE           0x001F
	#define COLOR_GREEN          0x07E0
	#define COLOR_RED            0xF800
	#define COLOR_WHITE          0xFFFF
	#define COLOR_CYAN           0x07FF
	#define COLOR_MAGENTA        0xF81F
	#define COLOR_YELLOW         0xFFE0
	#define COLOR_GREY           0xF7DE

	/* sajat szin (R, G, B: 0-255) */
	#define RGB2COLOR(r,g,b)     ((uint16_t)((((r)&0xF8)<<8)|(((g)&0xFC)<<3)|(((b)&0xF8)>>3)))


	/* A HSV terben megadott szin R5G6B5 kodjat adja vissza (H: 0-359, S: 0-100, V: 0-100). */
	uint16_t hsv2rgb565(uint16_t h, uint8_t s, uint8_t v);

	/* A megadott szinnel kitolti az aktiv reteget. */
	void gFill(uint16_t color);

	/* A megadott helyre rajzol egy megadott szinu pixelt. */
	inline void gDrawPixel(uint16_t x, uint16_t y, uint16_t color){*((volatile uint16_t*)(fbAddr+(2*((y*WIDTH)+x))))=color;};

	/* Egyenest rajzol az (x1, y1) pontbol az (x2, y2) pontba a megadott szinnel (Bresenham-algoritmus). */
	void gDrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);

	/* Ures teglalapot rajzol (x es y a bal felso sarok, w es h a meret). */
	void gDrawRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

	/* Kitoltott teglalapot rajzol (x es y a bal felso sarok, w es h a meret).            *
	 * Vizszintes es fuggoleges vonalat is ezzel erdemes rajzolni, mert ez DMA-t hasznal! */
	void gDrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
	
	/* Korvonalat rajzol (cx, cy) kozepponttal, r sugarral (Bresenham-algoritmus).  */
	void gDrawCircle(uint16_t cx, uint16_t cy, uint16_t r, uint16_t color);

	/* Kitoltott kort rajzol (cx, cy) kozepponttal, r sugarral (Bresenham-algoritmus).  */
	void gDrawFilledCircle(uint16_t cx, uint16_t cy, uint16_t r, uint16_t color);

	/* Az atadott kepet kirajzolja a megadott helyre. */
	void gDrawImage(image_t *img, uint16_t x, uint16_t y);

	/* Kiir egy karaktert a megadott betutipussal a megadott helyre. Betuszin: fontColor, hatterszin: bgColor.  *
	 * Betumagassag: h, megfelel a betutipusnak! Visszaadja a kirajzolt karakter szelesseget.                   */
	uint8_t gPutChar(uint16_t x, uint16_t y, char c, const void *font, uint8_t h, uint16_t fontColor, uint16_t bgColor);

	/* Kiir egy nullterminalt karakterlancot a megadott betutipussal a megadott helyre. Betuszin: fontColor, hatterszin: bgColor.  *
	 * Betumagassag: h, megfelel a betutipusnak!                                                                                   */
	void gPutString(uint16_t x, uint16_t y, char *str, const void *font, uint8_t h, uint16_t fontColor, uint16_t bgColor);

#endif
