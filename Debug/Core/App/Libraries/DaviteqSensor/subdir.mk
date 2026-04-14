################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/App/Libraries/DaviteqSensor/davitedSensor.c \
../Core/App/Libraries/DaviteqSensor/ringbuffer.c 

C_DEPS += \
./Core/App/Libraries/DaviteqSensor/davitedSensor.d \
./Core/App/Libraries/DaviteqSensor/ringbuffer.d 

OBJS += \
./Core/App/Libraries/DaviteqSensor/davitedSensor.o \
./Core/App/Libraries/DaviteqSensor/ringbuffer.o 


# Each subdirectory must supply rules for building sources it contributes
Core/App/Libraries/DaviteqSensor/%.o Core/App/Libraries/DaviteqSensor/%.su Core/App/Libraries/DaviteqSensor/%.cyclo: ../Core/App/Libraries/DaviteqSensor/%.c Core/App/Libraries/DaviteqSensor/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Core/App/Libraries/Sen66 -I../FATFS/Target -I../FATFS/App -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-App-2f-Libraries-2f-DaviteqSensor

clean-Core-2f-App-2f-Libraries-2f-DaviteqSensor:
	-$(RM) ./Core/App/Libraries/DaviteqSensor/davitedSensor.cyclo ./Core/App/Libraries/DaviteqSensor/davitedSensor.d ./Core/App/Libraries/DaviteqSensor/davitedSensor.o ./Core/App/Libraries/DaviteqSensor/davitedSensor.su ./Core/App/Libraries/DaviteqSensor/ringbuffer.cyclo ./Core/App/Libraries/DaviteqSensor/ringbuffer.d ./Core/App/Libraries/DaviteqSensor/ringbuffer.o ./Core/App/Libraries/DaviteqSensor/ringbuffer.su

.PHONY: clean-Core-2f-App-2f-Libraries-2f-DaviteqSensor

