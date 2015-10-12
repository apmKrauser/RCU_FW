/*
 * RadarControl.h
 *
 *  Created on: 14.09.2015
 *      Author: SpecialK
 */

#ifndef RADARCONTROL_H_
#define RADARCONTROL_H_

#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "commands.h"

#define RX_STREAM_BUFFER_SIZE 2048
#define ADC_BUFFER_SIZE 2048


volatile uint8_t RxStreamBuffer[RX_STREAM_BUFFER_SIZE];
volatile uint16_t RxStreamBufferBytes;
uint16_t ADC1Buffer[ADC_BUFFER_SIZE];
uint16_t ADC2Buffer[ADC_BUFFER_SIZE];

bool checkAndProcessCommand();
void processCommand(Command_Struct cmd);
void waitSendBufferUart();
void awaitDAQComplete();
void setFilterBaseFreq (uint32_t freq);
uint32_t setVCOFreq(uint32_t freq);
void setVCOOffset(uint32_t freq);
void startDAQ();
void sendBufferUart(uint8_t *pData, uint16_t Size);
void sendOk(bool ok);

// IRQ Handlers
void UART_DMA_Done_IRQHandler();
void HAL_UART_RxByte_IRQHandler(UART_HandleTypeDef *huart);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

// hard fault
void HALT(const char* str);

#endif /* RADARCONTROL_H_ */
