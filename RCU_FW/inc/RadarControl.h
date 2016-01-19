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


// Pin definitions
#define Pin_LED /* PD0 */ GPIOD_BASE, GPIO_PIN_0
// Signal Multiplexer
#define Pin_S0 /* PC7 */ GPIOC_BASE, GPIO_PIN_7
#define Pin_S1 /* PC8 */ GPIOC_BASE, GPIO_PIN_8
#define Pin_S2 /* PC9 */ GPIOC_BASE, GPIO_PIN_9
// Power selection
#define Pin_Mod1 /* PD8 */  GPIOD_BASE, GPIO_PIN_8
#define Pin_Mod2 /* PD9 */  GPIOD_BASE, GPIO_PIN_9
#define Pin_Mod3 /* PD10 */ GPIOD_BASE, GPIO_PIN_10
#define Pin_Mod4 /* PD11 */ GPIOD_BASE, GPIO_PIN_11
#define Pin_Mod5 /* PD12 */ GPIOD_BASE, GPIO_PIN_12
#define Pin_Mod6 /* PD13 */ GPIOD_BASE, GPIO_PIN_13
// LTC6602
#define Pin_GAIN0 /* PB0 */ GPIOB_BASE, GPIO_PIN_0
#define Pin_GAIN1 /* PB1 */ GPIOB_BASE, GPIO_PIN_1
#define Pin_HPF0  /* PA6 */ GPIOA_BASE, GPIO_PIN_6
#define Pin_HPF1  /* PA7 */ GPIOA_BASE, GPIO_PIN_7
#define Pin_LPF0  /* PC4 */ GPIOC_BASE, GPIO_PIN_4
#define Pin_LPF1  /* PC5 */ GPIOC_BASE, GPIO_PIN_5

#define MIN(X,Y) (((X) < (Y) )? (X) : (Y))
#define MAX(X,Y) (((X) > (Y) )? (X) : (Y))

volatile uint8_t RxStreamBuffer[RX_STREAM_BUFFER_SIZE];
volatile uint16_t RxStreamBufferBytes;
extern uint16_t ADC1Buffer[ADC_BUFFER_SIZE];
extern uint16_t ADC2Buffer[ADC_BUFFER_SIZE];
extern uint16_t ADC1BufferDelay;
extern uint16_t ADC2BufferDelay;


bool checkAndProcessCommand();
void processCommand(Command_Struct cmd);
void waitSendBufferUart();
void awaitDAQComplete();
void setFilterBaseFreq (uint32_t freq);
uint32_t setVCOFreq(uint32_t freq);
void setFilterGain (FilterGain_t gain);
void setFilterBandpass (FilterHighPassBaseDiv_t highp, FilterLowPassBaseDiv_t lowp);
void setModulePowerSwitches (uint8_t moduleBitmask);
void MPlexSelectModule (uint8_t idx);
void setADCRate(uint32_t freqid);
void setVCOOffset(uint32_t freq);
void startDAQ();
void sendBufferUart(uint8_t *pData, uint16_t Size);
void sendUARTOk(bool ok);
void sendUARTUInt32(uint32_t val);
void startUARTRxIT();
uint32_t constrain (uint32_t value, uint32_t border1, uint32_t border2);
uint32_t map (uint32_t value, uint32_t origin_from, uint32_t origin_to, uint32_t target_from, uint32_t target_to);

// IRQ Handlers
void UART_DMA_Done_IRQHandler();
void DAQ_DMA_Done_IRQHandler(ADC_HandleTypeDef *hadc);
void HAL_UART_RxByte_IRQHandler(UART_HandleTypeDef *huart);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

// hard fault
void HALT(const char* str);

#endif /* RADARCONTROL_H_ */
