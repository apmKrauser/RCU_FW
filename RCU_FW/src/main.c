/**
 ******************************************************************************
 * File Name          : main.c
 * Description        : Main program body
 ******************************************************************************
 *
 *
 * Parts of code include belong to STMicroelectronics
 * see copyright notice below
 *
 * COPYRIGHT(c) 2015 STMicroelectronics
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

// Includes ------------------------------------------------------------------
#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "systemclock.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "i2c.h"
#include "rng.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "stm32_hal_legacy.h"
#include "config.h"
#include "commands.h"
#include "RadarControl.h"
// #include <stdio.h>



#ifdef __GNUC__
// when small printf set to 'yes' calls __io_putchar(int ch)
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
//#define ARM_MATH_CM4 ? use fpu?

// Private variables ---------------------------------------------------------
uint32_t ret;

// Private function prototypes -----------------------------------------------
void SystemClock_Config(void);
void SysInits();

// -------------------------------- MAIN -------------------------------------
int main(void)
{

	// --- Initialization ---
	SysInits();
	HAL_Delay(1000);
	printf("Hello world ... \r\n");


	// --- main loop ---
	while (1)
	{

		HAL_GPIO_WritePin(GPIOD_BASE, (1<<13), GPIO_PIN_SET);
		checkAndProcessCommand();
		HAL_GPIO_WritePin(GPIOD_BASE, (1<<13), GPIO_PIN_RESET);

		printf("Test ... \r\n");
		ret = setVCOFreq(3200);  // check adc.prescaler !! check dma int led blick
		setVCOOffset(128);    // check sConfig.SamplingTime
		setFilterBaseFreq(2048);
		//HAL_GPIO_WritePin(GPIOD_BASE, (1<<13), GPIO_PIN_SET);
		HAL_Delay(500);

	} // main loop

}


void SysInits()
{
	// Reset of all peripherals, Initializes the Flash interface and the Systick.
	HAL_Init();

	// Configure the system clock
	SystemClock_Config();

	// Initialize all configured peripherals
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_ADC1_Init();
	MX_ADC2_Init();
	MX_DAC_Init();
	MX_I2C1_Init();
	MX_I2C2_Init();
	MX_RNG_Init();
	MX_TIM1_Init();
	MX_TIM6_Init();
	MX_USART1_UART_Init();
	MX_USART3_UART_Init();

	// Enables ADC and starts conversion of the regular channels.
	if( HAL_ADC_Start(&hadc1) != HAL_OK)
		HALT("=> [init]: ADC1 startup failure");
	if( HAL_ADC_Start(&hadc2) != HAL_OK)
		HALT("=> [init]: ADC1 startup failure");

	// Startup DAC trigger timer
	HAL_TIM_Base_Start(&htim6);

	// request byte from UART (needed for interrupt to occur)
	startUARTRxIT();

}



PUTCHAR_PROTOTYPE
{
	return (ITM_SendChar(ch));
}




#ifdef USE_FULL_ASSERT

/**
 * @brief Reports the name of the source file and the source line number
 * where the assert_param error has occurred.
 * @param file: pointer to the source file name
 * @param line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */

}

#endif

