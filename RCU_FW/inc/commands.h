/*
 * commands.h
 *
 *  Created on: 12.10.2015
 *      Author: SpecialK
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_


#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "config.h"

#define COMMAND_SIZE 13 // 2+4+4+4
#define CommandNOOP_init {0x00, 0x00, 0x00, 0x00}

typedef enum {
	CMD_NOOP            = 0x00,
	CMD_GetADC1Buffer   = 0x01,
	CMD_GetADC2Buffer   = 0x02,
	CMD_GetAndSendADC1  = 0x03,
	CMD_GetAndSendADC2  = 0x04,
	CMD_GetFFTBuffer    = 0x05,
	CMD_ConfigVCO		= 0x11,
	CMD_ConfigFilter	= 0x12,
	CMD_ConfigADCRate   = 0x13,
	CMD_ProcessData     = 0x20,
	CMD_StreamToBuffer  = 0x41
} RxCommand_t;

typedef enum { // uart/i2c receive mode
	RxMode_GetCommands,
	RxMode_RxStream  // fill RxStreamBuffer
} RxMode_t;

typedef struct
{
	uint8_t command;
	uint32_t param1;
	uint32_t param2;
	uint32_t param3;
} Command_Struct;


// LTC6602 Filter Gains
typedef enum {
	Filter_00dB = 0x00,
	Filter_12dB = 0x01,
	Filter_24dB = 0x02,
	Filter_30dB = 0x03
} FilterGain_t;

// Value which base frequency is divided by
typedef enum {
	Filter_LowPass_Div600    = 0x00,
	Filter_LowPass_Div300    = 0x01,
	Filter_LowPass_Div100    = 0x02,
	Filter_LowPass_Div100Max = 0x03
} FilterLowPassBaseDiv_t;

// same with highpass
typedef enum {
	Filter_HighPass_Div6000 = 0x00,
	Filter_HighPass_Div3000 = 0x01,
	Filter_HighPass_Div1000 = 0x02,
	Filter_HighPass_Bypass  = 0x03
} FilterHighPassBaseDiv_t;

Command_Struct CommandNOOP;

bool cmdCheckRxBuffer(Command_Struct* cmd, uint8_t* RxBuffer, uint16_t BufferLen);

#endif /* COMMANDS_H_ */
