################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOS/portable/GCC/ARM_CM0/port.c 

OBJS += \
./FreeRTOS/portable/GCC/ARM_CM0/port.o 

C_DEPS += \
./FreeRTOS/portable/GCC/ARM_CM0/port.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/portable/GCC/ARM_CM0/%.o FreeRTOS/portable/GCC/ARM_CM0/%.su FreeRTOS/portable/GCC/ARM_CM0/%.cyclo: ../FreeRTOS/portable/GCC/ARM_CM0/%.c FreeRTOS/portable/GCC/ARM_CM0/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DSTM32F0xB -c -I"C:/Users/Raffael/Downloads/GITHUB/FactoryIO_Final-First-Version/FactoryIO_Final-First-Version/app/inc" -I"C:/Users/Raffael/Downloads/GITHUB/FactoryIO_Final-First-Version/FactoryIO_Final-First-Version/FreeRTOS/portable/GCC/ARM_CM0" -I"C:/Users/Raffael/Downloads/GITHUB/FactoryIO_Final-First-Version/FactoryIO_Final-First-Version/FreeRTOS/include" -I"C:/Users/Raffael/Downloads/GITHUB/FactoryIO_Final-First-Version/FactoryIO_Final-First-Version/TraceRecorder/Include" -I"C:/Users/Raffael/Downloads/GITHUB/FactoryIO_Final-First-Version/FactoryIO_Final-First-Version/TraceRecorder/streamports/Jlink_RTT/include" -I"C:/Users/Raffael/Downloads/GITHUB/FactoryIO_Final-First-Version/FactoryIO_Final-First-Version/TraceRecorder/config" -I"C:/Users/Raffael/Downloads/GITHUB/FactoryIO_Final-First-Version/FactoryIO_Final-First-Version/bsp/inc" -I"C:/Users/Raffael/Downloads/GITHUB/FactoryIO_Final-First-Version/FactoryIO_Final-First-Version/cmsis/core" -I"C:/Users/Raffael/Downloads/GITHUB/FactoryIO_Final-First-Version/FactoryIO_Final-First-Version/cmsis/device/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-FreeRTOS-2f-portable-2f-GCC-2f-ARM_CM0

clean-FreeRTOS-2f-portable-2f-GCC-2f-ARM_CM0:
	-$(RM) ./FreeRTOS/portable/GCC/ARM_CM0/port.cyclo ./FreeRTOS/portable/GCC/ARM_CM0/port.d ./FreeRTOS/portable/GCC/ARM_CM0/port.o ./FreeRTOS/portable/GCC/ARM_CM0/port.su

.PHONY: clean-FreeRTOS-2f-portable-2f-GCC-2f-ARM_CM0

