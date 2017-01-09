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
#include "signal_processing.h"

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
uint16_t SignalBuffer[SIGNAL_BUFFER_SIZE] = {0};
float32_t FFTBuffer[2*ADC_FFT_SIZE] = {0.0};
uint16_t ADC1BufferDelay = 0;
uint16_t ADC2BufferDelay = 0;

RxMode_t RxMode = RxMode_GetCommands;
Command_Struct CurrentCommand = CommandNOOP_init;



bool checkAndProcessCommand()
{
	if (RxMode != RxMode_GetCommands) return false;
	if (cmdCheckRxBuffer(&CurrentCommand, (uint8_t*) &UART_RXBuffer, UART_RXBufferBytes))
	{
		  UART_RXBufferBytes = 0;
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
			//ADC_BUFFER_SIZE*2 <- adc buffer = 16bit array
			sendBufferUart((uint8_t*) &ADC1Buffer, ADC_BUFFER_SIZE*2);
			break;
		case CMD_GetADC2Buffer:
			sendBufferUart((uint8_t*) &ADC2Buffer, ADC_BUFFER_SIZE*2);
			break;
		case CMD_GetAndSendADC1:
			startDAQ();
			awaitDAQComplete();
			sendBufferUart((uint8_t*) &ADC1Buffer, ADC_BUFFER_SIZE*2);
			break;
		case CMD_GetAndSendADC2:
			startDAQ();
			awaitDAQComplete();
			sendBufferUart((uint8_t*) &ADC2Buffer, ADC_BUFFER_SIZE*2);
			break;
		case CMD_GetFFTBuffer:
			// Todo: FLOAT Buffer VERschicekn !!!!! VORSICht !!!!!!!!!!!!!!!
			// send 16 bit buffer implemented in CMD_ProcessData
			sendBufferUart((uint8_t*) &FFTBuffer, ADC_FFT_SIZE*4);
			break;
		case CMD_ConfigVCO:
			freq = setVCOFreq(cmd.param1);
			setVCOOffset(cmd.param2);
			// return actual frequency
			sendUARTUInt32(freq);
			break;
		case CMD_ConfigFilter:
			setFilterBaseFreq(cmd.param1);
			setFilterGain(cmd.param2);
			setFilterBandpass((cmd.param3 & 0x03), (cmd.param3 >> 2) & 0x03);
			sendUARTOk(true);
			break;
		case CMD_ConfigADCRate:
			setADCRate(cmd.param1);
			startDAQ();
			awaitDAQComplete();
			sendUARTOk(true);
			break;
		case CMD_ProcessData:
			//runCFFT(ADC1Buffer, ADC1BufferDelay,
			//		ADC2Buffer, ADC2BufferDelay, FFTBuffer);
			startDAQ();
			awaitDAQComplete();
			runRFFT(ADC1Buffer, ADC1BufferDelay, FFTBuffer);
			normalize16Bit(FFTBuffer, SignalBuffer);
			// Todo: check SIGNAL_BUFFER_SIZE = ADC_BUFFER_SIZE
			sendBufferUart((uint8_t*) &SignalBuffer, SIGNAL_BUFFER_SIZE*2);
			break;
		case CMD_StreamToBuffer:
			RxMode = RxMode_RxStream;
			ADC1BufferDelay = 0;
			ADC2BufferDelay = 0;
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
	HAL_UART_Transmit_DMA(&huart1, pData, Size);
//	HAL_UART_DMAResume(&huart1);
}

void sendUARTUInt32(uint32_t val)
{
	HAL_UART_Transmit(&huart1, (uint8_t*)&val, 4, 1000);
}

void sendUARTOk(bool ok)
{
	uint8_t msgRet = ok ? 0x00 : 0xFF;
	HAL_UART_Transmit(&huart1, (uint8_t*)&msgRet, 1, 1000);

}

void setFilterBaseFreq (uint32_t freq)
{
// 0V .. 3V => 40MHz .. 100MHz
	uint32_t v_out;
	v_out = map(freq, FILTER_BASEFREQ_MIN, FILTER_BASEFREQ_MAX,
			FILTER_BASEFREQ_MIN_DAC, FILTER_BASEFREQ_MAX_DAC);
	HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, v_out);
	HAL_DAC_Start(&hdac,DAC_CHANNEL_2);
}

void setFilterGain (FilterGain_t gain)
{
	HAL_GPIO_WritePin(Pin_GAIN0, (gain & 0b01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Pin_GAIN1, (gain & 0b10) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void setFilterBandpass (FilterHighPassBaseDiv_t highp, FilterLowPassBaseDiv_t lowp)
{
	// High pass
	HAL_GPIO_WritePin(Pin_HPF0, (highp & 0b01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Pin_HPF1, (highp & 0b10) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	// High pass
	HAL_GPIO_WritePin(Pin_LPF0, (lowp & 0b01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Pin_LPF1, (lowp & 0b10) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void setModulePowerSwitches (uint8_t moduleBitmask)
{

}

void MPlexSelectModule (uint8_t idx)
{

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

void setADCRate(uint32_t freqid)
{
	ADC_ChannelConfTypeDef sConfig1;
	ADC_ChannelConfTypeDef sConfig2;
	sConfig1.Channel = ADC_CHANNEL_1;
	sConfig2.Channel = ADC_CHANNEL_2;
	sConfig1.Rank = 1;
	sConfig2.Rank = 1;

	switch(freqid)
	{
		case 1:
			sConfig1.SamplingTime = ADC_SAMPLETIME_480CYCLES;
			sConfig2.SamplingTime = ADC_SAMPLETIME_480CYCLES;
			#ifdef VERBOSE_DEBUG
				printf("ADC SamplingTime set to %u\r\n", 480);
			#endif
			break;
		case 2:
			sConfig1.SamplingTime = ADC_SAMPLETIME_144CYCLES;
			sConfig2.SamplingTime = ADC_SAMPLETIME_144CYCLES;
			#ifdef VERBOSE_DEBUG
				printf("ADC SamplingTime set to %u\r\n", 144);
			#endif
			break;
		case 3:
			sConfig1.SamplingTime = ADC_SAMPLETIME_112CYCLES;
			sConfig2.SamplingTime = ADC_SAMPLETIME_112CYCLES;
			#ifdef VERBOSE_DEBUG
				printf("ADC SamplingTime set to %u\r\n", 112);
			#endif
			break;
		case 4:
			sConfig1.SamplingTime = ADC_SAMPLETIME_84CYCLES;
			sConfig2.SamplingTime = ADC_SAMPLETIME_84CYCLES;
			#ifdef VERBOSE_DEBUG
				printf("ADC SamplingTime set to %u\r\n", 84);
			#endif
			break;
		case 5:
			sConfig1.SamplingTime = ADC_SAMPLETIME_56CYCLES;
			sConfig2.SamplingTime = ADC_SAMPLETIME_56CYCLES;
			#ifdef VERBOSE_DEBUG
				printf("ADC SamplingTime set to %u\r\n", 56);
			#endif
			break;
		case 6:
			sConfig1.SamplingTime = ADC_SAMPLETIME_28CYCLES;
			sConfig2.SamplingTime = ADC_SAMPLETIME_28CYCLES;
			#ifdef VERBOSE_DEBUG
				printf("ADC SamplingTime set to %u\r\n", 28);
			#endif
			break;
		case 7:
			sConfig1.SamplingTime = ADC_SAMPLETIME_15CYCLES;
			sConfig2.SamplingTime = ADC_SAMPLETIME_15CYCLES;
			#ifdef VERBOSE_DEBUG
				printf("ADC SamplingTime set to %u\r\n", 15);
			#endif
			break;
		case 8:
			sConfig1.SamplingTime = ADC_SAMPLETIME_3CYCLES;
			sConfig2.SamplingTime = ADC_SAMPLETIME_3CYCLES;
			#ifdef VERBOSE_DEBUG
				printf("ADC SamplingTime set to %u\r\n", 3);
			#endif
			break;
		default:
			#ifdef VERBOSE_DEBUG
				printf("ADC ignored undefined SamplingTime.");
			#endif
			// abort channel config
			return;
	}
	HAL_ADC_ConfigChannel(&hadc1, &sConfig1);
	HAL_ADC_ConfigChannel(&hadc2, &sConfig2);
}

void setVCOOffset(uint32_t offset)
{
	HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, offset);
}

void startDAQ()
{
	IsBusy_ADC2 = IsBusy_ADC1 = true;
	// Reset DAC Timer
	__HAL_TIM_SetCounter(&htim6, 0);
	// Enables ADC DMA
	if (HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADC1Buffer, ADC_BUFFER_SIZE) != HAL_OK)
		HALT("=> ADC_DMA startup failure");
	if (HAL_ADC_Start_DMA(&hadc2, (uint32_t*)ADC2Buffer, ADC_BUFFER_SIZE) != HAL_OK)
		HALT("=> ADC_DMA startup failure");
	// Enables ADC and starts conversion of the regular channels.
	if( HAL_ADC_Start(&hadc1) != HAL_OK)
		HALT("=> [init]: ADC1 startup failure");
	if( HAL_ADC_Start(&hadc2) != HAL_OK)
		HALT("=> [init]: ADC1 startup failure");
	//  Start VCO modulating DAC
	if (HAL_DAC_Start(&hdac,DAC_CHANNEL_1) != HAL_OK)
		HALT("=> DAC1 startup failure");
}

void startUARTRxIT()
{
	// request byte from UART (needed for interrupt to occur)
	HAL_UART_Receive_IT(&huart1,(uint8_t*) &uart_rx_byte, 1);
}


void DAQ_DMA_Done_IRQHandler(ADC_HandleTypeDef *hadc)
{
	// Stop ADC + DAC(VCO)
	HAL_ADC_Stop_DMA(&hadc1);
	HAL_ADC_Stop_DMA(&hadc2);
	HAL_ADC_Stop(&hadc1);
	HAL_ADC_Stop(&hadc2);
	HAL_DAC_Stop(&hdac, DAC_CHANNEL_1);
	IsBusy_ADC1 = IsBusy_ADC2 = false;
}

void UART_DMA_Done_IRQHandler()
{
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
			// Todo: use RX STREAM adc1 +2  !!!!
			// rxstream ist 8 bit, adc ist 16 bit
			RxStreamBuffer[RxStreamBufferBytes] = uart_rx_byte;
			RxStreamBufferBytes++;
			if (RxStreamBufferBytes >= RX_STREAM_BUFFER_SIZE)
			{
				RxStreamBufferBytes = 0;
				RxMode = RxMode_GetCommands;
				// copy to ADC Buffer
				memcpy(ADC1Buffer, RxStreamBuffer, ADC_BUFFER_SIZE);
				memcpy(ADC2Buffer, RxStreamBuffer+ADC_BUFFER_SIZE, ADC_BUFFER_SIZE);
				HAL_GPIO_WritePin(GPIOD_BASE, (1<<12), GPIO_PIN_RESET);
			}
			break;
	}
	// receive next byte
	HAL_UART_Receive_IT(&huart1,(uint8_t*) &uart_rx_byte, 1);
}

void HALT(const char* str)
{
	printf(str);
	while(1);
}

uint32_t map (uint32_t value, uint32_t origin_from, uint32_t origin_to, uint32_t target_from, uint32_t target_to)
{
	uint32_t ret;
	value = constrain(value, origin_from, origin_to);
	double f = 1.0 * ((int64_t)target_to - (int64_t)target_from) / ((int64_t)origin_to - (int64_t)origin_from);
	ret = (uint32_t)( target_from + f * (value - origin_from));
	// constrain just in case
	return constrain(ret, target_from, target_to);
}

uint32_t constrain (uint32_t value, uint32_t border1, uint32_t border2)
{
	uint32_t bmin;
	uint32_t bmax;

	bmin = MIN(border1,border2);
	bmax = MAX(border1,border2);
	if (value < bmin)
		value = bmin;
	if (value > bmax)
		value = bmax;
	return value;
}



