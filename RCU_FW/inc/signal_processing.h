/*
 * signal_processing.h
 *
 *  Created on: 13.01.2016
 *      Author: SpecialK
 */

#ifndef SIGNAL_PROCESSING_H_
#define SIGNAL_PROCESSING_H_

#include "config.h"

void runCFFT (uint16_t* inSignalIF, uint16_t inSignalIFDelay, uint16_t* inSignalQ, uint16_t inSignalQDelay, float32_t* outBuffer);

void runRFFT (uint16_t* inSignalIF, uint16_t inSignalIFDelay, float32_t* outBuffer);

// prepare FFT Signal for sending over UART
void normalize16Bit (float32_t* inBuffer, uint16_t* outBuffer);

#endif /* SIGNAL_PROCESSING_H_ */
