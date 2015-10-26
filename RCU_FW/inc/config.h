/*
 * config.h
 *
 *  Created on: 30.09.2015
 *      Author: SpecialK
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#define VERBOSE_DEBUG

#define RX_STREAM_BUFFER_SIZE 2048
#define ADC_BUFFER_SIZE       2048
#define ADC_FFT_SIZE          2048

#define FILTER_BASEFREQ_MIN       40000 // kHz
#define FILTER_BASEFREQ_MAX      100000 // kHz
#define FILTER_BASEFREQ_MIN_DAC    3724 // dac value
#define FILTER_BASEFREQ_MAX_DAC       0 // dac value

#endif /* CONFIG_H_ */
