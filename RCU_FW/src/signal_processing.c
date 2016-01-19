/*
 * signal_processing.c
 *
 *  Created on: 13.01.2016
 *      Author: SpecialK
 */

#include "signal_processing.h"
#include "config.h"

// Buffer comprising pairs of real and imaginary values
// obtained from ADC capture
float32_t inBufFFT[2*ADC_FFT_SIZE] = {0.0};
arm_rfft_fast_instance_f32 rfft_instance;

// inBuff will be overwritten?
void runCFFT (uint16_t* inSignalIF, uint16_t inSignalIFDelay,
		uint16_t* inSignalQ, uint16_t inSignalQDelay, float32_t* outBuffer)
{
	// fill complex buffer with inphase(adc1) and quadrature(adc2) values
	for (int i = 0; i < ADC_FFT_SIZE; ++i) {
		inBufFFT[i*2]   = (float32_t) inSignalIF[i + inSignalIFDelay];
		inBufFFT[i*2+1] = (float32_t) inSignalQ[i + inSignalQDelay];
	}

	// run fft
	arm_cfft_f32(&cfft_instance, inBufFFT, 0, 1);

	// Process the data through the Complex Magniture Module
	arm_cmplx_mag_f32(inBufFFT, outBuffer, ADC_FFT_SIZE);

	// get highest value and index
	//arm_max_f32(outBuffer, ADC_FFT_SIZE, maxValue, maxIndex);
}

void runRFFT (uint16_t* inSignalIF, uint16_t inSignalIFDelay, float32_t* outBuffer)
{
	arm_rfft_fast_init_f32(&rfft_instance, ADC_FFT_SIZE);

	// fill float buffer with inphase(adc1) values
	for (int i = 0; i < ADC_FFT_SIZE; ++i) {
		inBufFFT[i]   = (float32_t) inSignalIF[i + inSignalIFDelay];
	}
	arm_rfft_fast_f32(&rfft_instance, inBufFFT, outBuffer, 0);
}

*/
