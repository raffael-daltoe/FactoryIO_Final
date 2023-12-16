################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TraceRecorder/trcInternalBuffer.c \
../TraceRecorder/trcKernelPort.c \
../TraceRecorder/trcSnapshotRecorder.c \
../TraceRecorder/trcStreamingRecorder.c 

OBJS += \
./TraceRecorder/trcInternalBuffer.o \
./TraceRecorder/trcKernelPort.o \
./TraceRecorder/trcSnapshotRecorder.o \
./TraceRecorder/trcStreamingRecorder.o 

C_DEPS += \
./TraceRecorder/trcInternalBuffer.d \
./TraceRecorder/trcKernelPort.d \
./TraceRecorder/trcSnapshotRecorder.d \
./TraceRecorder/trcStreamingRecorder.d 


# Each subdirectory must supply rules for building sources it contributes
TraceRecorder/%.o TraceRecorder/%.su TraceRecorder/%.cyclo: ../TraceRecorder/%.c TraceRecorder/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DSTM32F0xB -c -I"C:/Users/Raffael/Downloads/TP3/app/inc" -I"C:/Users/Raffael/Downloads/TP3/TraceRecorder/Include" -I"C:/Users/Raffael/Downloads/TP3/FreeRTOS/include" -I"C:/Users/Raffael/Downloads/TP3/FreeRTOS/portable/GCC/ARM_CM0" -I"C:/Users/Raffael/Downloads/TP3/bsp/inc" -I"C:/Users/Raffael/Downloads/TP3/cmsis/core" -I"C:/Users/Raffael/Downloads/TP3/cmsis/device/inc" -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-TraceRecorder

clean-TraceRecorder:
	-$(RM) ./TraceRecorder/trcInternalBuffer.cyclo ./TraceRecorder/trcInternalBuffer.d ./TraceRecorder/trcInternalBuffer.o ./TraceRecorder/trcInternalBuffer.su ./TraceRecorder/trcKernelPort.cyclo ./TraceRecorder/trcKernelPort.d ./TraceRecorder/trcKernelPort.o ./TraceRecorder/trcKernelPort.su ./TraceRecorder/trcSnapshotRecorder.cyclo ./TraceRecorder/trcSnapshotRecorder.d ./TraceRecorder/trcSnapshotRecorder.o ./TraceRecorder/trcSnapshotRecorder.su ./TraceRecorder/trcStreamingRecorder.cyclo ./TraceRecorder/trcStreamingRecorder.d ./TraceRecorder/trcStreamingRecorder.o ./TraceRecorder/trcStreamingRecorder.su

.PHONY: clean-TraceRecorder

