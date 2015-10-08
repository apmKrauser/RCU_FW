/*
 * RadarControl.h
 *
 *  Created on: 14.09.2015
 *      Author: SpecialK
 */

#ifndef RADARCONTROL_H_
#define RADARCONTROL_H_

#include "stm32f4xx_hal.h"
#include "stm32_hal_legacy.h"
#include "dac.h"
#include "tim.h"

enum {
	CMD_NOOP            = 0x00,
	CMD_GetADC1Buffer   = 0x01,
	CMD_GetADC2Buffer   = 0x02
};

void setFilterBaseFreq (uint32_t freq);

uint32_t setVCOFreq(uint32_t freq);

void setVCOOffset(uint32_t freq);

void waitSendBufferUart();

void sendBufferUart(uint8_t *pData, uint16_t Size);

void UART_DMA_Done_IRQHandler();

#endif /* RADARCONTROL_H_ */
