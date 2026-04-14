################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/App/Libraries/Posrget/Postget.cpp 

OBJS += \
./Core/App/Libraries/Posrget/Postget.o 

CPP_DEPS += \
./Core/App/Libraries/Posrget/Postget.d 


# Each subdirectory must supply rules for building sources it contributes
Core/App/Libraries/Posrget/%.o Core/App/Libraries/Posrget/%.su Core/App/Libraries/Posrget/%.cyclo: ../Core/App/Libraries/Posrget/%.cpp Core/App/Libraries/Posrget/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-App-2f-Libraries-2f-Posrget

clean-Core-2f-App-2f-Libraries-2f-Posrget:
	-$(RM) ./Core/App/Libraries/Posrget/Postget.cyclo ./Core/App/Libraries/Posrget/Postget.d ./Core/App/Libraries/Posrget/Postget.o ./Core/App/Libraries/Posrget/Postget.su

.PHONY: clean-Core-2f-App-2f-Libraries-2f-Posrget

