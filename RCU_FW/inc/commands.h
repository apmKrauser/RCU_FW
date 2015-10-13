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
	CMD_ConfigVCO		= 0x11,
	CMD_ConfigFilter	= 0x12,
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

Command_Struct CommandNOOP;

bool cmdCheckRxBuffer(Command_Struct* cmd, uint8_t* RxBuffer, uint16_t BufferLen);

#endif /* COMMANDS_H_ */
