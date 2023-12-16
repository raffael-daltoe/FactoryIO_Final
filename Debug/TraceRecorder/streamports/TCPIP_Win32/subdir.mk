################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TraceRecorder/streamports/TCPIP_Win32/trcStreamPort.c 

OBJS += \
./TraceRecorder/streamports/TCPIP_Win32/trcStreamPort.o 

C_DEPS += \
./TraceRecorder/streamports/TCPIP_Win32/trcStreamPort.d 


# Each subdirectory must supply rules for building sources it contributes
TraceRecorder/streamports/TCPIP_Win32/%.o TraceRecorder/streamports/TCPIP_Win32/%.su TraceRecorder/streamports/TCPIP_Win32/%.cyclo: ../TraceRecorder/streamports/TCPIP_Win32/%.c TraceRecorder/streamports/TCPIP_Win32/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DSTM32F0xB -c -I"C:/Users/Raffael/Downloads/TP3/TraceRecorder/include" -I"C:/Users/Raffael/Downloads/TP3/app/inc" -I"C:/Users/Raffael/Downloads/TP3/TraceRecorder/streamports/Jlink_RTT/include" -I"C:/Users/Raffael/Downloads/TP3/FreeRTOS/include" -I"C:/Users/Raffael/Downloads/TP3/FreeRTOS/portable/GCC/ARM_CM0" -I"C:/Users/Raffael/Downloads/TP3/bsp/inc" -I"C:/Users/Raffael/Downloads/TP3/cmsis/core" -I"C:/Users/Raffael/Downloads/TP3/cmsis/device/inc" -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-TraceRecorder-2f-streamports-2f-TCPIP_Win32

clean-TraceRecorder-2f-streamports-2f-TCPIP_Win32:
	-$(RM) ./TraceRecorder/streamports/TCPIP_Win32/trcStreamPort.cyclo ./TraceRecorder/streamports/TCPIP_Win32/trcStreamPort.d ./TraceRecorder/streamports/TCPIP_Win32/trcStreamPort.o ./TraceRecorder/streamports/TCPIP_Win32/trcStreamPort.su

.PHONY: clean-TraceRecorder-2f-streamports-2f-TCPIP_Win32

