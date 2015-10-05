/*
 * RadarControl.cpp
 *
 *  Created on: 14.09.2015
 *      Author: SpecialK
 */

#include "RadarControl.h"
#include "config.h"



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
