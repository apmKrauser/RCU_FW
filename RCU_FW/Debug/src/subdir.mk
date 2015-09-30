################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/RadarControl.c \
../src/adc.c \
../src/dac.c \
../src/dma.c \
../src/gpio.c \
../src/i2c.c \
../src/main.c \
../src/rng.c \
../src/stm32f4xx_hal_msp.c \
../src/stm32f4xx_it.c \
../src/syscalls.c \
../src/system_stm32f4xx.c \
../src/test.c \
../src/tim.c \
../src/usart.c 

OBJS += \
./src/RadarControl.o \
./src/adc.o \
./src/dac.o \
./src/dma.o \
./src/gpio.o \
./src/i2c.o \
./src/main.o \
./src/rng.o \
./src/stm32f4xx_hal_msp.o \
./src/stm32f4xx_it.o \
./src/syscalls.o \
./src/system_stm32f4xx.o \
./src/test.o \
./src/tim.o \
./src/usart.o 

C_DEPS += \
./src/RadarControl.d \
./src/adc.d \
./src/dac.d \
./src/dma.d \
./src/gpio.d \
./src/i2c.d \
./src/main.d \
./src/rng.d \
./src/stm32f4xx_hal_msp.d \
./src/stm32f4xx_it.d \
./src/syscalls.d \
./src/system_stm32f4xx.d \
./src/test.d \
./src/tim.d \
./src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407VGTx -DSTM32F4 -DSTM32F4DISCOVERY -DSTM32 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -I"C:/develop/RCU_FW_Test2/stm32f4discovery_hal_lib" -I"C:/develop/RCU_FW_Test2/RCU_FW/inc" -I"C:/develop/RCU_FW_Test2/stm32f4discovery_hal_lib/CMSIS/core" -I"C:/develop/RCU_FW_Test2/stm32f4discovery_hal_lib/CMSIS/device" -I"C:/develop/RCU_FW_Test2/stm32f4discovery_hal_lib/HAL_Driver/Inc/Legacy" -I"C:/develop/RCU_FW_Test2/stm32f4discovery_hal_lib/HAL_Driver/Inc" -I"C:/develop/RCU_FW_Test2/stm32f4discovery_hal_lib/Utilities/Components/ampire480272" -I"C:/develop/RCU_FW_Test2/stm32f4discovery_hal_lib/Utilities/Components/ampire640480" -I"C:/develop/RCU_FW_Test2/stm32f4discovery_hal_lib/Utilities/Components/Common" -I"C:/develop/RCU_FW_Test2/stm32f4discovery_hal_lib/Utilities/Components/cs43l22" -I"C:/develop/RCU_FW_Test2/stm32f4discovery_hal_lib/Utilities/Components/exc7200" -I"C:/develop/RCU_FW_Test2/stm32f4discovery_hal_lib/Utilities/Components/ft6x06" -I"C:/develop/RCU_FW_Test2/stm32f4discovery_hal_lib/Utilities/Components/ili9325" -I"C:/develop/RCU_FW_Test2/stm32f4discovery_hal_lib/Utilities/Components/ili9341" -I"C:/develop/RCU_FW_Test2/stm32f4discovery_hal_lib/Utilities/Components/l3gd20" -I"C:/develop/RCU_FW_Test2/stm32f4discovery_hal_lib/Utilities/Components/lis302dl" -I"C:/develop/RCU_FW_Test2/stm32f4discovery_hal_lib/Utilities/Components/lis3dsh" -I"C:/develop/RCU_FW_Test2/stm32f4discovery_hal_lib/Utilities/Components/lsm303dlhc" -I"C:/develop/RCU_FW_Test2/stm32f4discovery_hal_lib/Utilities/Components/mfxstm32l152" -I"C:/develop/RCU_FW_Test2/stm32f4discovery_hal_lib/Utilities/Components/n25q128a" -I"C:/develop/RCU_FW_Test2/stm32f4discovery_hal_lib/Utilities/Components/n25q256a" -I"C:/develop/RCU_FW_Test2/stm32f4discovery_hal_lib/Utilities/Components/n25q512a" -I"C:/develop/RCU_FW_Test2/stm32f4discovery_hal_lib/Utilities/Components/otm8009a" -I"C:/develop/RCU_FW_Test2/stm32f4discovery_hal_lib/Utilities/Components/ov2640" -I"C:/develop/RCU_FW_Test2/stm32f4discovery_hal_lib/Utilities/Components/s25fl512s" -I"C:/develop/RCU_FW_Test2/stm32f4discovery_hal_lib/Utilities/Components/s5k5cag" -I"C:/develop/RCU_FW_Test2/stm32f4discovery_hal_lib/Utilities/Components/st7735" -I"C:/develop/RCU_FW_Test2/stm32f4discovery_hal_lib/Utilities/Components/stmpe1600" -I"C:/develop/RCU_FW_Test2/stm32f4discovery_hal_lib/Utilities/Components/stmpe811" -I"C:/develop/RCU_FW_Test2/stm32f4discovery_hal_lib/Utilities/Components/ts3510" -I"C:/develop/RCU_FW_Test2/stm32f4discovery_hal_lib/Utilities/Components/wm8994" -I"C:/develop/RCU_FW_Test2/stm32f4discovery_hal_lib/Utilities" -I"C:/develop/RCU_FW_Test2/stm32f4discovery_hal_lib/Utilities/STM32F4-Discovery" -I"C:/develop/RCU_FW_Test2/stm32f4discovery_hal_lib/HAL_Driver/Src" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


