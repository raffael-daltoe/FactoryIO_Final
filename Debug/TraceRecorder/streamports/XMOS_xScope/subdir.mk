################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TraceRecorder/streamports/XMOS_xScope/trcStreamPort.c 

OBJS += \
./TraceRecorder/streamports/XMOS_xScope/trcStreamPort.o 

C_DEPS += \
./TraceRecorder/streamports/XMOS_xScope/trcStreamPort.d 


# Each subdirectory must supply rules for building sources it contributes
TraceRecorder/streamports/XMOS_xScope/%.o TraceRecorder/streamports/XMOS_xScope/%.su TraceRecorder/streamports/XMOS_xScope/%.cyclo: ../TraceRecorder/streamports/XMOS_xScope/%.c TraceRecorder/streamports/XMOS_xScope/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DSTM32F0xB -c -I"C:/Users/Raffael/Downloads/TP3/TraceRecorder/include" -I"C:/Users/Raffael/Downloads/TP3/app/inc" -I"C:/Users/Raffael/Downloads/TP3/TraceRecorder/streamports/Jlink_RTT/include" -I"C:/Users/Raffael/Downloads/TP3/FreeRTOS/include" -I"C:/Users/Raffael/Downloads/TP3/FreeRTOS/portable/GCC/ARM_CM0" -I"C:/Users/Raffael/Downloads/TP3/bsp/inc" -I"C:/Users/Raffael/Downloads/TP3/cmsis/core" -I"C:/Users/Raffael/Downloads/TP3/cmsis/device/inc" -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-TraceRecorder-2f-streamports-2f-XMOS_xScope

clean-TraceRecorder-2f-streamports-2f-XMOS_xScope:
	-$(RM) ./TraceRecorder/streamports/XMOS_xScope/trcStreamPort.cyclo ./TraceRecorder/streamports/XMOS_xScope/trcStreamPort.d ./TraceRecorder/streamports/XMOS_xScope/trcStreamPort.o ./TraceRecorder/streamports/XMOS_xScope/trcStreamPort.su

.PHONY: clean-TraceRecorder-2f-streamports-2f-XMOS_xScope

