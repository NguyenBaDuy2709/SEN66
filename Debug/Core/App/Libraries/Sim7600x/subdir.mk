################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/App/Libraries/Sim7600x/Sim7600x.c 

C_DEPS += \
./Core/App/Libraries/Sim7600x/Sim7600x.d 

OBJS += \
./Core/App/Libraries/Sim7600x/Sim7600x.o 


# Each subdirectory must supply rules for building sources it contributes
Core/App/Libraries/Sim7600x/%.o Core/App/Libraries/Sim7600x/%.su Core/App/Libraries/Sim7600x/%.cyclo: ../Core/App/Libraries/Sim7600x/%.c Core/App/Libraries/Sim7600x/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Core/App/Libraries/Sen66 -I../FATFS/Target -I../FATFS/App -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-App-2f-Libraries-2f-Sim7600x

clean-Core-2f-App-2f-Libraries-2f-Sim7600x:
	-$(RM) ./Core/App/Libraries/Sim7600x/Sim7600x.cyclo ./Core/App/Libraries/Sim7600x/Sim7600x.d ./Core/App/Libraries/Sim7600x/Sim7600x.o ./Core/App/Libraries/Sim7600x/Sim7600x.su

.PHONY: clean-Core-2f-App-2f-Libraries-2f-Sim7600x

