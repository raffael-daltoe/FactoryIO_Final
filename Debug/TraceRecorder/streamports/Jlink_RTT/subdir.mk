################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TraceRecorder/streamports/Jlink_RTT/SEGGER_RTT.c \
../TraceRecorder/streamports/Jlink_RTT/trcStreamingPort.c 

OBJS += \
./TraceRecorder/streamports/Jlink_RTT/SEGGER_RTT.o \
./TraceRecorder/streamports/Jlink_RTT/trcStreamingPort.o 

C_DEPS += \
./TraceRecorder/streamports/Jlink_RTT/SEGGER_RTT.d \
./TraceRecorder/streamports/Jlink_RTT/trcStreamingPort.d 


# Each subdirectory must supply rules for building sources it contributes
TraceRecorder/streamports/Jlink_RTT/%.o TraceRecorder/streamports/Jlink_RTT/%.su TraceRecorder/streamports/Jlink_RTT/%.cyclo: ../TraceRecorder/streamports/Jlink_RTT/%.c TraceRecorder/streamports/Jlink_RTT/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DSTM32F0xB -c -I"C:/Users/Raffael/Downloads/GITHUB/FactoryIO_Final-First-Version/FactoryIO_Final-First-Version/app/inc" -I"C:/Users/Raffael/Downloads/GITHUB/FactoryIO_Final-First-Version/FactoryIO_Final-First-Version/FreeRTOS/portable/GCC/ARM_CM0" -I"C:/Users/Raffael/Downloads/GITHUB/FactoryIO_Final-First-Version/FactoryIO_Final-First-Version/FreeRTOS/include" -I"C:/Users/Raffael/Downloads/GITHUB/FactoryIO_Final-First-Version/FactoryIO_Final-First-Version/TraceRecorder/Include" -I"C:/Users/Raffael/Downloads/GITHUB/FactoryIO_Final-First-Version/FactoryIO_Final-First-Version/TraceRecorder/streamports/Jlink_RTT/include" -I"C:/Users/Raffael/Downloads/GITHUB/FactoryIO_Final-First-Version/FactoryIO_Final-First-Version/TraceRecorder/config" -I"C:/Users/Raffael/Downloads/GITHUB/FactoryIO_Final-First-Version/FactoryIO_Final-First-Version/bsp/inc" -I"C:/Users/Raffael/Downloads/GITHUB/FactoryIO_Final-First-Version/FactoryIO_Final-First-Version/cmsis/core" -I"C:/Users/Raffael/Downloads/GITHUB/FactoryIO_Final-First-Version/FactoryIO_Final-First-Version/cmsis/device/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-TraceRecorder-2f-streamports-2f-Jlink_RTT

clean-TraceRecorder-2f-streamports-2f-Jlink_RTT:
	-$(RM) ./TraceRecorder/streamports/Jlink_RTT/SEGGER_RTT.cyclo ./TraceRecorder/streamports/Jlink_RTT/SEGGER_RTT.d ./TraceRecorder/streamports/Jlink_RTT/SEGGER_RTT.o ./TraceRecorder/streamports/Jlink_RTT/SEGGER_RTT.su ./TraceRecorder/streamports/Jlink_RTT/trcStreamingPort.cyclo ./TraceRecorder/streamports/Jlink_RTT/trcStreamingPort.d ./TraceRecorder/streamports/Jlink_RTT/trcStreamingPort.o ./TraceRecorder/streamports/Jlink_RTT/trcStreamingPort.su

.PHONY: clean-TraceRecorder-2f-streamports-2f-Jlink_RTT

