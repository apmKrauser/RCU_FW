/*
 * commands.c
 *
 *  Created on: 12.10.2015
 *      Author: SpecialK
 */

#include "commands.h"

Command_Struct CommandNOOP = CommandNOOP_init;

bool cmdCheckRxBuffer(Command_Struct* cmd, uint8_t* RxBuffer, uint16_t BufferLen)
{
	if (BufferLen >= COMMAND_SIZE)
	{
		cmd->command = RxBuffer[0];
		// little endian
		cmd->param1 =   RxBuffer[1];
		cmd->param1 |= (RxBuffer[2] << 8);
		cmd->param1 |= (RxBuffer[3] << 16);
		cmd->param1 |= (RxBuffer[4] << 24);
		cmd->param2 =   RxBuffer[5];
		cmd->param2 |= (RxBuffer[6] << 8);
		cmd->param2 |= (RxBuffer[7] << 16);
		cmd->param2 |= (RxBuffer[8] << 24);
		cmd->param3 =   RxBuffer[9];
		cmd->param3 |= (RxBuffer[10] << 8);
		cmd->param3 |= (RxBuffer[11] << 16);
		cmd->param3 |= (RxBuffer[12] << 24);
		return true;
	}
	else return false;
}

