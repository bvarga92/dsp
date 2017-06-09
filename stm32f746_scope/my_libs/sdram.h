#ifndef _SDRAM_H_
#define _SDRAM_H_

	#include "stm32f7xx.h"
	#include "stm32f7xx_hal.h"
	#include "stm32f7xx_hal_cortex.h"
	#include "stm32f7xx_hal_sdram.h"

	/* a memoriaterulet kezdocime */
	#define SDRAM_BASE_ADDR                          ((uint32_t)0xC0000000)

	/* makrok a konfiguracios regiszterhez */
	#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
	#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
	#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
	#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
	#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
	#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
	#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
	#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
	#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
	#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
	#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)

	/* SDRAM inicializalasa. Siker eseten 0-val ter vissza. */
	uint8_t sdramInit(void);

	/* Irasi es olvasasi muveletek (8 MB --> cimzes: 0x000000-0x800000, csak illesztetten). */
	void sdramWrite8(uint32_t addr, uint8_t data);
	uint8_t sdramRead8(uint32_t addr);
	void sdramWrite16(uint32_t addr, uint16_t data);
	uint16_t sdramRead16(uint32_t addr);
	void sdramWrite32(uint32_t addr, uint32_t data);
	uint32_t sdramRead32(uint32_t addr);
	void sdramWriteBuffer32(uint32_t* buffer, uint32_t startAddr, uint32_t len);
	void sdramReadBuffer32(uint32_t* buffer, uint32_t startAddr, uint32_t len);
	void sdramWriteBuffer32DMA(uint32_t* buffer, uint32_t startAddr, uint32_t len);

#endif
