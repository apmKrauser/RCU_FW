/*
 * RadarControl.cpp
 *
 *  Created on: 14.09.2015
 *      Author: SpecialK
 */

#include "RadarControl.h"



void setFilterBaseFreq (uint32_t freq)
{
	HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, freq);
	HAL_DAC_Start(&hdac,DAC_CHANNEL_2);
}

void setVCOFreq(uint32_t freq)
{
	//int	fdfds &hdac;
	//__HAL_TIM_SetAutoreload(&hdac, 0);
	//__HAL_TIM_SET_AUTORELOAD(&hdac, 0);
	//HAL_TIM_Base_Stop(&htim6);
	//HAL_TIM_Base_DeInit(&htim6);
	// 168M/2/512/period=freq
	htim6.Init.Period = (uint32_t)(164063 / freq);
	HAL_TIM_Base_Init(&htim6);
	//HAL_TIM_Base_Start(&htim6);

}

void setVCOOffset(uint32_t offset)
{
	HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, offset);
}
