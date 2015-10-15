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
void setADCRate(uint32_t freqid);
void setVCOOffset(uint32_t freq);
void startDAQ();
void sendBufferUart(uint8_t *pData, uint16_t Size);
void sendUARTOk(bool ok);
void sendUARTUInt32(uint32_t val);
void startUARTRxIT();

// IRQ Handlers
void UART_DMA_Done_IRQHandler();
void DAQ_DMA_Done_IRQHandler(ADC_HandleTypeDef *hadc);
void HAL_UART_RxByte_IRQHandler(UART_HandleTypeDef *huart);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

// hard fault
void HALT(const char* str);

#endif /* RADARCONTROL_H_ */
