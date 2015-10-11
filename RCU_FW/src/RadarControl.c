/*
 * RadarControl.cpp
 *
 *  Created on: 14.09.2015
 *      Author: SpecialK
 */

#include <stdbool.h>
#include "RadarControl.h"
#include "stm32f4xx_hal.h"
#include "config.h"
#include "usart.h"

// local variables
bool IsBusy_UART_DMA = false;
// todo: remove
char UART_Buffer2[100] = "Sende ADC Puffer\r\n";

// wait for last send buffer over uart request to complete
void waitSendBufferUart()
{
	// loop until dma irq called
	while (IsBusy_UART_DMA);
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

void UART_DMA_Done_IRQHandler()
{
	// todo: remove
	HAL_UART_DMAPause(&huart3);
	//HAL_UART_DMAPause(&huart1);
	IsBusy_UART_DMA = false;
}

void HAL_UART_RxByte_IRQHandler(UART_HandleTypeDef *huart)
{
	if ((char)uart_rx_byte == '1')
		sendBufferUart((uint8_t *)&UART_Buffer2, 100);
	//HAL_UART_Receive_IT(&huart1,(uint8_t*) &uart_rx_byte, 1);
	// todo: remove
	HAL_UART_Receive_IT(&huart3,(uint8_t*) &uart_rx_byte, 1);
	//__HAL_UART_FLUSH_DRREGISTER()
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
