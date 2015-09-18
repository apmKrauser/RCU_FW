/*
 * RadarControl.h
 *
 *  Created on: 14.09.2015
 *      Author: SpecialK
 */

#ifndef RADARCONTROL_H_
#define RADARCONTROL_H_

#include "stm32f4xx_hal.h"
#include "stm32_hal_legacy.h"
#include "dac.h"
#include "tim.h"

void setFilterBaseFreq (uint32_t freq);

void setVCOFreq(uint32_t freq);

void setVCOOffset(uint32_t freq);

#endif /* RADARCONTROL_H_ */
