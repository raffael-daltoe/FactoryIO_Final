################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TraceRecorder/streamports/ARM_ITM/trcStreamPort.c 

OBJS += \
./TraceRecorder/streamports/ARM_ITM/trcStreamPort.o 

C_DEPS += \
./TraceRecorder/streamports/ARM_ITM/trcStreamPort.d 


# Each subdirectory must supply rules for building sources it contributes
TraceRecorder/streamports/ARM_ITM/%.o TraceRecorder/streamports/ARM_ITM/%.su TraceRecorder/streamports/ARM_ITM/%.cyclo: ../TraceRecorder/streamports/ARM_ITM/%.c TraceRecorder/streamports/ARM_ITM/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DSTM32F0xB -c -I"C:/Users/Raffael/Downloads/TP3/TraceRecorder/include" -I"C:/Users/Raffael/Downloads/TP3/app/inc" -I"C:/Users/Raffael/Downloads/TP3/TraceRecorder/streamports/Jlink_RTT/include" -I"C:/Users/Raffael/Downloads/TP3/FreeRTOS/include" -I"C:/Users/Raffael/Downloads/TP3/FreeRTOS/portable/GCC/ARM_CM0" -I"C:/Users/Raffael/Downloads/TP3/bsp/inc" -I"C:/Users/Raffael/Downloads/TP3/cmsis/core" -I"C:/Users/Raffael/Downloads/TP3/cmsis/device/inc" -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-TraceRecorder-2f-streamports-2f-ARM_ITM

clean-TraceRecorder-2f-streamports-2f-ARM_ITM:
	-$(RM) ./TraceRecorder/streamports/ARM_ITM/trcStreamPort.cyclo ./TraceRecorder/streamports/ARM_ITM/trcStreamPort.d ./TraceRecorder/streamports/ARM_ITM/trcStreamPort.o ./TraceRecorder/streamports/ARM_ITM/trcStreamPort.su

.PHONY: clean-TraceRecorder-2f-streamports-2f-ARM_ITM

