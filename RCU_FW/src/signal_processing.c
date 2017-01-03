/*
 * signal_processing.c
 *
 *  Created on: 13.01.2016
 *      Author: SpecialK
 */

#include "signal_processing.h"
#include "config.h"
#include "stm32f4xx_hal.h"


// Buffer comprising pairs of real and imaginary values
// obtained from ADC capture
float32_t inBufFFT[2*ADC_FFT_SIZE] = {0.0};
// precalc window function
float32_t HannWindow[ADC_FFT_SIZE] = {0.0};
arm_rfft_fast_instance_f32 rfft_instance;


// inBuff will be overwritten?
void runCFFT (uint16_t* inSignalIF, uint16_t inSignalIFDelay,uint16_t* inSignalQ,
		uint16_t inSignalQDelay, float32_t* outBuffer)
{
	const static arm_cfft_instance_f32 *S;

	// initialize cfft instance
	switch (ADC_FFT_SIZE) {
	    case 16:
	      S = &arm_cfft_sR_f32_len16;
	      break;
	    case 32:
	      S = &arm_cfft_sR_f32_len32;
	      break;
	    case 64:
	      S = &arm_cfft_sR_f32_len64;
	      break;
	    case 128:
	      S = &arm_cfft_sR_f32_len128;
	      break;
	    case 256:
	      S = &arm_cfft_sR_f32_len256;
	      break;
	    case 512:
	      S = &arm_cfft_sR_f32_len512;
	      break;
	    case 1024:
	      S = &arm_cfft_sR_f32_len1024;
	      break;
	    case 2048:
	      S = &arm_cfft_sR_f32_len2048;
	      break;
	    case 4096:
	      S = &arm_cfft_sR_f32_len4096;
	      break;
	}


	// todo: window function !! !!!!!!!!!!!!!!!!!

	// fill complex buffer with inphase(adc1) and quadrature(adc2) values
	for (int i = 0; i < ADC_FFT_SIZE; ++i) {
		inBufFFT[i*2]   = (float32_t) inSignalIF[i + inSignalIFDelay];
		inBufFFT[i*2+1] = (float32_t) inSignalQ[i + inSignalQDelay];
	}

	// run fft
	arm_cfft_f32(S, inBufFFT, 0, 1);

	// Process the data through the Complex Magniture Module
	arm_cmplx_mag_f32(inBufFFT, outBuffer, ADC_FFT_SIZE);

	// get highest value and index
	//float32_t *maxValue;
	//float32_t *maxIndex;
	//arm_max_f32(outBuffer, ADC_FFT_SIZE, maxValue, maxIndex);

}

void runRFFT (uint16_t* inSignalIF, uint16_t inSignalIFDelay, float32_t* outBuffer)
{
	arm_rfft_fast_init_f32(&rfft_instance, ADC_FFT_SIZE);

	// fill float buffer with inphase(adc1) values
	for (int i = 0; i < ADC_FFT_SIZE; ++i) {
		inBufFFT[i] = ((float32_t) inSignalIF[i + inSignalIFDelay]) * HannWindow[i];
	}
	arm_rfft_fast_f32(&rfft_instance, inBufFFT, outBuffer, 0);
	// remove const peak
	outBuffer[0] = 0;
}

void normalize16Bit (float32_t* inBuffer, uint16_t* outBuffer)
{
	// get highest value and index
	float32_t maxValue;
	uint32_t maxIndex;
	arm_max_f32(inBuffer, ADC_FFT_SIZE, &maxValue, &maxIndex);

	// Normalize to 65000
	if (0 == maxValue) maxValue = 1;
	for (int i = 0; i < ADC_FFT_SIZE; ++i) {
		// warning: side effect on inBuffer
		if (inBuffer[i] < 0) inBuffer[i] = 0;
		outBuffer[i] = (uint16_t) (inBuffer[i] / maxValue * 65000.0);
	}
	for (int i = ADC_FFT_SIZE; i < SIGNAL_BUFFER_SIZE; ++i) {
		outBuffer[i]   = 0;
	}
}

void precalcHannWindow ()
{
	for(uint16_t i = 0; i < ADC_FFT_SIZE; i++)
	{
		HannWindow[i] = 0.5 - (0.5 * cos ( (2.0 * PI * ((float32_t)i)) / (ADC_FFT_SIZE - 1)));
	}
}


