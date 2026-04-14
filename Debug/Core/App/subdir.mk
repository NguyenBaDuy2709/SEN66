################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/App/05_RTC.cpp \
../Core/App/06_SDCard.cpp \
../Core/App/07_Relay.cpp \
../Core/App/ChuongTrinhChinh.cpp 

C_SRCS += \
../Core/App/01_Flags.c \
../Core/App/sen66_app.c 

C_DEPS += \
./Core/App/01_Flags.d \
./Core/App/sen66_app.d 

OBJS += \
./Core/App/01_Flags.o \
./Core/App/05_RTC.o \
./Core/App/06_SDCard.o \
./Core/App/07_Relay.o \
./Core/App/ChuongTrinhChinh.o \
./Core/App/sen66_app.o 

CPP_DEPS += \
./Core/App/05_RTC.d \
./Core/App/06_SDCard.d \
./Core/App/07_Relay.d \
./Core/App/ChuongTrinhChinh.d 


# Each subdirectory must supply rules for building sources it contributes
Core/App/%.o Core/App/%.su Core/App/%.cyclo: ../Core/App/%.c Core/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Core/App/Libraries/Sen66 -I../FATFS/Target -I../FATFS/App -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/App/%.o Core/App/%.su Core/App/%.cyclo: ../Core/App/%.cpp Core/App/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-App

clean-Core-2f-App:
	-$(RM) ./Core/App/01_Flags.cyclo ./Core/App/01_Flags.d ./Core/App/01_Flags.o ./Core/App/01_Flags.su ./Core/App/05_RTC.cyclo ./Core/App/05_RTC.d ./Core/App/05_RTC.o ./Core/App/05_RTC.su ./Core/App/06_SDCard.cyclo ./Core/App/06_SDCard.d ./Core/App/06_SDCard.o ./Core/App/06_SDCard.su ./Core/App/07_Relay.cyclo ./Core/App/07_Relay.d ./Core/App/07_Relay.o ./Core/App/07_Relay.su ./Core/App/ChuongTrinhChinh.cyclo ./Core/App/ChuongTrinhChinh.d ./Core/App/ChuongTrinhChinh.o ./Core/App/ChuongTrinhChinh.su ./Core/App/sen66_app.cyclo ./Core/App/sen66_app.d ./Core/App/sen66_app.o ./Core/App/sen66_app.su

.PHONY: clean-Core-2f-App

