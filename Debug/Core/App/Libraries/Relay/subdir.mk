################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/App/Libraries/Relay/Relay.c \
../Core/App/Libraries/Relay/Relaytime.c 

C_DEPS += \
./Core/App/Libraries/Relay/Relay.d \
./Core/App/Libraries/Relay/Relaytime.d 

OBJS += \
./Core/App/Libraries/Relay/Relay.o \
./Core/App/Libraries/Relay/Relaytime.o 


# Each subdirectory must supply rules for building sources it contributes
Core/App/Libraries/Relay/%.o Core/App/Libraries/Relay/%.su Core/App/Libraries/Relay/%.cyclo: ../Core/App/Libraries/Relay/%.c Core/App/Libraries/Relay/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Core/App/Libraries/Sen66 -I../FATFS/Target -I../FATFS/App -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-App-2f-Libraries-2f-Relay

clean-Core-2f-App-2f-Libraries-2f-Relay:
	-$(RM) ./Core/App/Libraries/Relay/Relay.cyclo ./Core/App/Libraries/Relay/Relay.d ./Core/App/Libraries/Relay/Relay.o ./Core/App/Libraries/Relay/Relay.su ./Core/App/Libraries/Relay/Relaytime.cyclo ./Core/App/Libraries/Relay/Relaytime.d ./Core/App/Libraries/Relay/Relaytime.o ./Core/App/Libraries/Relay/Relaytime.su

.PHONY: clean-Core-2f-App-2f-Libraries-2f-Relay

