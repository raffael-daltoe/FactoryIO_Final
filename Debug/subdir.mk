################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../sycall.c 

OBJS += \
./sycall.o 

C_DEPS += \
./sycall.d 


# Each subdirectory must supply rules for building sources it contributes
%.o %.su %.cyclo: ../%.c subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DSTM32F0xB -c -I"C:/Users/Raffael/Downloads/GITHUB/FactoryIO_Final-First-Version/FactoryIO_Final-First-Version/app/inc" -I"C:/Users/Raffael/Downloads/GITHUB/FactoryIO_Final-First-Version/FactoryIO_Final-First-Version/FreeRTOS/portable/GCC/ARM_CM0" -I"C:/Users/Raffael/Downloads/GITHUB/FactoryIO_Final-First-Version/FactoryIO_Final-First-Version/FreeRTOS/include" -I"C:/Users/Raffael/Downloads/GITHUB/FactoryIO_Final-First-Version/FactoryIO_Final-First-Version/TraceRecorder/Include" -I"C:/Users/Raffael/Downloads/GITHUB/FactoryIO_Final-First-Version/FactoryIO_Final-First-Version/TraceRecorder/streamports/Jlink_RTT/include" -I"C:/Users/Raffael/Downloads/GITHUB/FactoryIO_Final-First-Version/FactoryIO_Final-First-Version/TraceRecorder/config" -I"C:/Users/Raffael/Downloads/GITHUB/FactoryIO_Final-First-Version/FactoryIO_Final-First-Version/bsp/inc" -I"C:/Users/Raffael/Downloads/GITHUB/FactoryIO_Final-First-Version/FactoryIO_Final-First-Version/cmsis/core" -I"C:/Users/Raffael/Downloads/GITHUB/FactoryIO_Final-First-Version/FactoryIO_Final-First-Version/cmsis/device/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean--2e-

clean--2e-:
	-$(RM) ./sycall.cyclo ./sycall.d ./sycall.o ./sycall.su

.PHONY: clean--2e-

