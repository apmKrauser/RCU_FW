/*
 * RadarControl.cpp
 *
 *  Created on: 14.09.2015
 *      Author: SpecialK
 */

#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "RadarControl.h"
#include "config.h"
#include "usart.h"
#include "commands.h"
#include "adc.h"
#include "dac.h"
#include "tim.h"

// variables
#define UART_RXBUFFER_SIZE 100

volatile bool IsBusy_UART_DMA = false;
volatile bool IsBusy_ADC1 = false;
volatile bool IsBusy_ADC2 = false;
volatile uint8_t uart_rx_byte;

volatile uint8_t UART_RXBuffer[UART_RXBUFFER_SIZE];
volatile uint16_t UART_RXBufferBytes = 0;

volatile uint16_t RxStreamBufferBytes = 0;

uint16_t ADC1Buffer[ADC_BUFFER_SIZE] = {0};
uint16_t ADC2Buffer[ADC_BUFFER_SIZE] = {0};

RxMode_t RxMode = RxMode_GetCommands;
Command_Struct CurrentCommand = CommandNOOP_init;



bool checkAndProcessCommand()
{
	if (RxMode != RxMode_GetCommands) return false;
	if (cmdCheckRxBuffer(&CurrentCommand, (uint8_t*) &UART_RXBuffer, RxStreamBufferBytes))
	{
		  RxStreamBufferBytes = 0;
		  processCommand(CurrentCommand);
		  return true;
	} else return false;
}

void processCommand(Command_Struct cmd)
{
    uint32_t freq;
	switch (cmd.command)
	{
		case CMD_NOOP:
			break;
		case CMD_GetADC1Buffer:
			sendBufferUart((uint8_t*) &ADC1Buffer, ADC_BUFFER_SIZE);
			break;
		case CMD_GetADC2Buffer:
			break;
		case CMD_GetAndSendADC1:
			startDAQ();
			awaitDAQComplete();
			sendBufferUart((uint8_t*) &ADC1Buffer, ADC_BUFFER_SIZE);
			break;
		case CMD_GetAndSendADC2:
			break;
		case CMD_ConfigVCO:
			freq = setVCOFreq(cmd.param1);
			setVCOOffset(cmd.param2);
			// return actual frequency
			sendUARTUInt32(freq);
			break;
		case CMD_ConfigFilter:
			break;
		case CMD_StreamToBuffer:
			RxMode = RxMode_RxStream;
			sendUARTOk(true);
			break;
	}
	CurrentCommand = CommandNOOP;
}


// wait for last send buffer over uart request to complete
void awaitSendBufferUart()
{
	// loop until dma irq called
	while (IsBusy_UART_DMA);
}

void awaitDAQComplete()
{
	while (IsBusy_ADC1 || IsBusy_ADC2);
	HAL_Delay(2);
}

// send buffer over uart non blocking using dma
void sendBufferUart(uint8_t *pData, uint16_t Size)
{
	IsBusy_UART_DMA = true;
	// todo: remove
	HAL_UART_Transmit_DMA(&huart3, pData, Size);
	HAL_UART_DMAResume(&huart3);
//	HAL_UART_Transmit_DMA(&huart1, pData, Size);
//	HAL_UART_DMAResume(&huart1);
}

void sendUARTUInt32(uint32_t val)
{
	HAL_UART_Transmit(&huart3, (uint8_t*)&val, 4, 1000);
}

void sendUARTOk(bool ok)
{
	uint8_t msgRet = ok ? 0x00 : 0xFF;
	// todo: remove uart3 -> uart 1
	HAL_UART_Transmit(&huart3, (uint8_t*)&msgRet, 1, 1000);

}

void setFilterBaseFreq (uint32_t freq)
{
	HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, freq);
	HAL_DAC_Start(&hdac,DAC_CHANNEL_2);
}

uint32_t setVCOFreq(uint32_t freq)
{
	uint32_t vcofreq;
	float periodFkt;
	uint32_t period;
	//int	fdfds &hdac;
	//__HAL_TIM_SetAutoreload(&hdac, 0);
	//__HAL_TIM_SET_AUTORELOAD(&hdac, 0);
	//HAL_TIM_Base_Stop(&htim6);
	//HAL_TIM_Base_DeInit(&htim6);
	// 168M/2/2048/period=freq
	periodFkt = (168000000.0/2.0/2.0/2048.0);
	period = (uint32_t)(periodFkt / freq) - 1;
	if (period < 0) period = 0;
	htim6.Init.Period = period;  // period-1?
	HAL_TIM_Base_Init(&htim6);
	vcofreq = (uint32_t)(periodFkt / (period + 1));
#ifdef VERBOSE_DEBUG
	printf("f_VCO set to %u\r\n", (unsigned int)vcofreq);
#endif
	return vcofreq;
	//HAL_TIM_Base_Start(&htim6);

}

void setVCOOffset(uint32_t offset)
{
	HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, offset);
}

void startDAQ()
{
	IsBusy_ADC2 = IsBusy_ADC1 = true;
	// Enables ADC DMA request
	if (HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADC1Buffer, ADC_BUFFER_SIZE) != HAL_OK)
		HALT("=> ADC_DMA startup failure");
	if (HAL_ADC_Start_DMA(&hadc2, (uint32_t*)ADC2Buffer, ADC_BUFFER_SIZE) != HAL_OK)
		HALT("=> ADC_DMA startup failure");
	//  Start VCO modulating DAC
	if (HAL_DAC_Start(&hdac,DAC_CHANNEL_1) != HAL_OK)
		HALT("=> DAC1 startup failure");
}

void startUARTRxIT()
{
	// request byte from UART (needed for interrupt to occur)
	// todo: remove 3 => 1
	HAL_UART_Receive_IT(&huart3,(uint8_t*) &uart_rx_byte, 1);
}

void UART_DMA_Done_IRQHandler()
{
	// todo: remove
	HAL_UART_DMAPause(&huart3);
	//HAL_UART_DMAPause(&huart1);
	IsBusy_UART_DMA = false;
}

void HAL_UART_RxByte_IRQHandler(UART_HandleTypeDef *huart)
{
	//__HAL_UART_FLUSH_DRREGISTER() // really needed? some examples use this
	switch(RxMode)
	{
		case RxMode_GetCommands:
			UART_RXBuffer[UART_RXBufferBytes] = uart_rx_byte;
			UART_RXBufferBytes++;
			UART_RXBufferBytes %= UART_RXBUFFER_SIZE;
			break;
		case RxMode_RxStream:
			RxStreamBuffer[RxStreamBufferBytes] = uart_rx_byte;
			RxStreamBufferBytes++;
			if (RxStreamBufferBytes >= RX_STREAM_BUFFER_SIZE)
			{
				RxStreamBufferBytes = 0;
				RxMode = RxMode_GetCommands;
			}
			break;
	}
	// receive next byte
	HAL_UART_Receive_IT(&huart3,(uint8_t*) &uart_rx_byte, 1);
}

void HALT(const char* str)
{
	printf(str);
	while(1);
}




