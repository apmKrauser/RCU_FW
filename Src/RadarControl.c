/*
 * RadarControl.cpp
 *
 *  Created on: 14.09.2015
 *      Author: SpecialK
 */

#include "RadarControl.h"



void setFilterBaseFreq (uint32_t freq)
{
	HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 0);
}

void setVCOFreq(uint32_t freq)
{
	//int	fdfds &hdac;
	//__HAL_TIM_SetAutoreload(&hdac, 0);
	__HAL_TIM_SetCompare(&hdac, DAC_CHANNEL_1,0);
	//__HAL_TIM_SET_AUTORELOAD(&hdac, 0);

}

void setVCOOffset(uint32_t offset)
{
	HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, offset);
}
