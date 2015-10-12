/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
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
/* Includes ------------------------------------------------------------------*/
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


/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
#ifdef __GNUC__
    // when small printf set to 'yes' calls __io_putchar(int ch)
    #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
    #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
//#define ARM_MATH_CM4 ? use fpu?
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
//volatile int ITM_RxBuffer = ITM_RXBUFFER_EMPTY;  /*  CMSIS Debug Input        */


char UART_Buffer[100] = "Test 0 Text\r\n";

//RadarControl RadarCtl;
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */


/* USER CODE END 0 */

int main(void)
{
  /* USER CODE BEGIN 1 */
	uint32_t ret;
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
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

  /* USER CODE BEGIN 2 */

  // -- Enables ADC and starts conversion of the regular channels.
  if( HAL_ADC_Start(&hadc1) != HAL_OK)
	  return 0;
  if( HAL_ADC_Start(&hadc2) != HAL_OK)
	  return 0;
  // -- Enables ADC DMA request
  if (HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADC1Buffer, 2048) != HAL_OK)
	  return 0;
  if (HAL_ADC_Start_DMA(&hadc2, (uint32_t*)ADC2Buffer, 2048) != HAL_OK)
	  return 0;
  //HAL_DMAEx_MultiBufferStart(&hdma_adc3, hadc1.Instance->DR, (uint32_t) Buffer1, (uint32_t) Buffer2, 10);
//  ADC_RegularChannelConfig(ADC1,ADC_Channel_12,2,ADC_SampleTime_480Cycles);
//  ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
  HAL_TIM_Base_Start(&htim6);
  HAL_DAC_Start(&hdac,DAC_CHANNEL_1);
  HAL_DAC_Start(&hdac,DAC_CHANNEL_2);
 // HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 0);
  //HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*)sine_wave_data, 32, DAC_ALIGN_12B_R);


  HAL_Delay(5000);
  //HAL_GPIO_WritePin(GPIOD_BASE, (1<<12), GPIO_PIN_SET);
  printf("Hello world ... \r\n");
  HAL_Delay(1000);
  // Trigger receivement of uart bytes
 // HAL_UART_Receive_IT(&huart1,(uint8_t*) &uart_rx_byte, 1);
  // todo: remove
 // HAL_UART_Receive_IT(&huart3,(uint8_t*) &uart_rx_byte, 1);
  //sendBufferUart((uint8_t *)&UART_Buffer, 100);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  checkAndProcessCommand();

	  //TIM Update Frequency = TIM Clock / (P * Q)
	  //Prescaler = P - 1, Period = Q - 1

	  printf("Test ... \r\n");
	  ret = setVCOFreq(3200);  // check adc.prescaler !! check dma int led blick
	  setVCOOffset(128);    // check sConfig.SamplingTime
	  setFilterBaseFreq(2048);
	  HAL_GPIO_WritePin(GPIOD_BASE, (1<<13), GPIO_PIN_SET);
	  HAL_Delay(5000);

//	  UART_Buffer[5]++;
//	  sendBufferUart(&UART_Buffer, 100);
	  //waitSendBufferUart()


	  printf("Besonderer Test ... \r\n");

	  setFilterBaseFreq(0);
	  setVCOFreq(2000);
	  HAL_GPIO_WritePin(GPIOD_BASE, (1<<13), GPIO_PIN_RESET);
	  HAL_Delay(5000);

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}



/* USER CODE BEGIN 4 */
PUTCHAR_PROTOTYPE
{
    return (ITM_SendChar(ch));
}

//int _write( int file, char *ptr, int len )
//{
//  int txCount;
//  (void)file;
//  for ( txCount = 0; txCount < len; txCount++)
//  {
//	  ITM_SendChar(ptr[txCount]);
//	  //HAL_UART_Transmit(&huart1, (uint8_t*)&ptr[txCount], 1, 10000);
//  }
//  return len;
//}



/* USER CODE END 4 */

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

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
