#ifndef _AUDIO_H_
#define _AUDIO_H_

	#include "stm32f7xx.h"
	#include "stm32f7xx_hal.h"
	#include "stm32f7xx_hal_cortex.h"
	#include "stm32f7xx_hal_i2c.h"
	#include "stm32f7xx_hal_sai.h"


	/* ki- es bemeneti eszkozok */
	#define OUTPUT_DEVICE_SPEAKER                 ((uint16_t)0x0001)
	#define OUTPUT_DEVICE_HEADPHONE               ((uint16_t)0x0002)
	#define OUTPUT_DEVICE_BOTH                    ((uint16_t)0x0003)
	#define OUTPUT_DEVICE_AUTO                    ((uint16_t)0x0004)
	#define INPUT_DEVICE_DIGITAL_MICROPHONE_1     ((uint16_t)0x0100)
	#define INPUT_DEVICE_DIGITAL_MICROPHONE_2     ((uint16_t)0x0200)
	#define INPUT_DEVICE_INPUT_LINE_1             ((uint16_t)0x0300)
	#define INPUT_DEVICE_INPUT_LINE_2             ((uint16_t)0x0400)

	/* mintaveteli frekvenciak */
	#define AUDIO_FREQUENCY_192K                  ((uint32_t)192000)
	#define AUDIO_FREQUENCY_96K                   ((uint32_t)96000)
	#define AUDIO_FREQUENCY_48K                   ((uint32_t)48000)
	#define AUDIO_FREQUENCY_44K                   ((uint32_t)44100)
	#define AUDIO_FREQUENCY_32K                   ((uint32_t)32000)
	#define AUDIO_FREQUENCY_22K                   ((uint32_t)22050)
	#define AUDIO_FREQUENCY_16K                   ((uint32_t)16000)
	#define AUDIO_FREQUENCY_11K                   ((uint32_t)11025)
	#define AUDIO_FREQUENCY_8K                    ((uint32_t)8000)


	/* interrupt flagek */
	extern volatile uint8_t inTransferHalfComplete;
	extern volatile uint8_t inTransferComplete;
	extern volatile uint8_t outTransferHalfComplete;
	extern volatile uint8_t outTransferComplete;


	/* Inicializalas bemenetkent. Eszkoz csak LINE_1 vagy MICROPHONE_2. */
	void audioInInit(uint16_t device, uint8_t volume, uint32_t samplingRate);

	/* Beallitja a felveteli buffert. */
	void audioSetInputBuffer(uint16_t *buffer, uint32_t length);

	/* Inicializalas kimenetkent. */
	void audioOutInit(uint16_t device, uint8_t volume, uint32_t samplingRate);

	/* Beallitja a lejatszasi buffert. Formatum: [L0, R0, L1, R1, ...]. */
	void audioSetOutputBuffer(uint16_t *buffer, uint32_t length);
	
	/* Inicializalas bemenetkent es kimenetkent. */
	void audioInOutInit(uint16_t deviceIn, uint16_t deviceOut, uint8_t volume, uint32_t samplingRate);

#endif
