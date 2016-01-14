/*
 * signal_processing.h
 *
 *  Created on: 13.01.2016
 *      Author: SpecialK
 */

#ifndef SIGNAL_PROCESSING_H_
#define SIGNAL_PROCESSING_H_

#include "arm_math.h"
#include "arm_const_structs.h"

void runCFFT (uint16_t* inSignalIF, uint16_t inSignalIFDelay, uint16_t* inSignalQ, uint16_t inSignalQDelay, float32_t* outBuffer)

void runRFFT (uint16_t* inSignalIF, uint16_t inSignalIFDelay, float32_t* outBuffer)

#endif /* SIGNAL_PROCESSING_H_ */
