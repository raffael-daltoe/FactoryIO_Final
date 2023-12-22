################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TraceRecorder/trcKernelPort.c \
../TraceRecorder/trcSnapshotRecorder.c \
../TraceRecorder/trcStreamingRecorder.c 

OBJS += \
./TraceRecorder/trcKernelPort.o \
./TraceRecorder/trcSnapshotRecorder.o \
./TraceRecorder/trcStreamingRecorder.o 

C_DEPS += \
./TraceRecorder/trcKernelPort.d \
./TraceRecorder/trcSnapshotRecorder.d \
./TraceRecorder/trcStreamingRecorder.d 


# Each subdirectory must supply rules for building sources it contributes
TraceRecorder/%.o TraceRecorder/%.su TraceRecorder/%.cyclo: ../TraceRecorder/%.c TraceRecorder/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DSTM32F0xB -c -I"C:/Users/Raffael/Downloads/GITHUB/FactoryIO_Final-First-Version/FactoryIO_Final-First-Version/app/inc" -I"C:/Users/Raffael/Downloads/GITHUB/FactoryIO_Final-First-Version/FactoryIO_Final-First-Version/FreeRTOS/portable/GCC/ARM_CM0" -I"C:/Users/Raffael/Downloads/GITHUB/FactoryIO_Final-First-Version/FactoryIO_Final-First-Version/FreeRTOS/include" -I"C:/Users/Raffael/Downloads/GITHUB/FactoryIO_Final-First-Version/FactoryIO_Final-First-Version/TraceRecorder/Include" -I"C:/Users/Raffael/Downloads/GITHUB/FactoryIO_Final-First-Version/FactoryIO_Final-First-Version/TraceRecorder/streamports/Jlink_RTT/include" -I"C:/Users/Raffael/Downloads/GITHUB/FactoryIO_Final-First-Version/FactoryIO_Final-First-Version/TraceRecorder/config" -I"C:/Users/Raffael/Downloads/GITHUB/FactoryIO_Final-First-Version/FactoryIO_Final-First-Version/bsp/inc" -I"C:/Users/Raffael/Downloads/GITHUB/FactoryIO_Final-First-Version/FactoryIO_Final-First-Version/cmsis/core" -I"C:/Users/Raffael/Downloads/GITHUB/FactoryIO_Final-First-Version/FactoryIO_Final-First-Version/cmsis/device/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-TraceRecorder

clean-TraceRecorder:
	-$(RM) ./TraceRecorder/trcKernelPort.cyclo ./TraceRecorder/trcKernelPort.d ./TraceRecorder/trcKernelPort.o ./TraceRecorder/trcKernelPort.su ./TraceRecorder/trcSnapshotRecorder.cyclo ./TraceRecorder/trcSnapshotRecorder.d ./TraceRecorder/trcSnapshotRecorder.o ./TraceRecorder/trcSnapshotRecorder.su ./TraceRecorder/trcStreamingRecorder.cyclo ./TraceRecorder/trcStreamingRecorder.d ./TraceRecorder/trcStreamingRecorder.o ./TraceRecorder/trcStreamingRecorder.su

.PHONY: clean-TraceRecorder

