/*
 * config.h
 *
 *  Created on: 30.09.2015
 *      Author: SpecialK
 */

#ifndef CONFIG_H_
#define CONFIG_H_

/* FPU present on STM32F4xx device */
#define __FPU_PRESENT   1
/* Use ARM MATH for Cortex-M4 */
#define ARM_MATH_CM4

#define VERBOSE_DEBUG

#define RX_STREAM_BUFFER_SIZE 4096
#define ADC_BUFFER_SIZE       4096
#define ADC_FFT_SIZE          2048
const arm_cfft_instance_f32 FFT_DESCR = { ADC_FFT_SIZE,
										  twiddleCoef_2048,
										  armBitRevIndexTable2048,
										  ARMBITREVINDEXTABLE2048_TABLE_LENGTH
										};
const arm_rfft_fast_instance_f32 RFFT_DESCR = { FFT_DESCR,
											    ADC_FFT_SIZE,
											    twiddleCoef_rfft_4096
											  };

#define FILTER_BASEFREQ_MIN       40000 // kHz
#define FILTER_BASEFREQ_MAX      100000 // kHz
#define FILTER_BASEFREQ_MIN_DAC    3724 // dac value
#define FILTER_BASEFREQ_MAX_DAC       0 // dac value

#endif /* CONFIG_H_ */
