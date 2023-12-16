################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TraceRecorder/extras/ExtensionExample/source/MyExtension.c \
../TraceRecorder/extras/ExtensionExample/source/main.c 

OBJS += \
./TraceRecorder/extras/ExtensionExample/source/MyExtension.o \
./TraceRecorder/extras/ExtensionExample/source/main.o 

C_DEPS += \
./TraceRecorder/extras/ExtensionExample/source/MyExtension.d \
./TraceRecorder/extras/ExtensionExample/source/main.d 


# Each subdirectory must supply rules for building sources it contributes
TraceRecorder/extras/ExtensionExample/source/%.o TraceRecorder/extras/ExtensionExample/source/%.su TraceRecorder/extras/ExtensionExample/source/%.cyclo: ../TraceRecorder/extras/ExtensionExample/source/%.c TraceRecorder/extras/ExtensionExample/source/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DSTM32F0xB -c -I"C:/Users/Raffael/Downloads/TP3/app/inc" -I"C:/Users/Raffael/Downloads/TP3/TraceRecorder/streamports/Jlink_RTT" -I"C:/Users/Raffael/Downloads/TP3/TraceRecorder/streamports/Jlink_RTT/include" -I"C:/Users/Raffael/Downloads/TP3/TraceRecorder" -I"C:/Users/Raffael/Downloads/TP3/bsp/inc" -I"C:/Users/Raffael/Downloads/TP3/cmsis/core" -I"C:/Users/Raffael/Downloads/TP3/cmsis/device/inc" -I"C:/Users/Raffael/Downloads/TP3/FreeRTOS/include" -I"C:/Users/Raffael/Downloads/TP3/FreeRTOS/portable/GCC/ARM_CM0" -I"C:/Users/Raffael/Downloads/TP3/TraceRecorder/include" -I"C:/Users/Raffael/Downloads/TP3/TraceRecorder/config" -I"C:/Users/Raffael/Downloads/TP3/TraceRecorder/streamports" -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-TraceRecorder-2f-extras-2f-ExtensionExample-2f-source

clean-TraceRecorder-2f-extras-2f-ExtensionExample-2f-source:
	-$(RM) ./TraceRecorder/extras/ExtensionExample/source/MyExtension.cyclo ./TraceRecorder/extras/ExtensionExample/source/MyExtension.d ./TraceRecorder/extras/ExtensionExample/source/MyExtension.o ./TraceRecorder/extras/ExtensionExample/source/MyExtension.su ./TraceRecorder/extras/ExtensionExample/source/main.cyclo ./TraceRecorder/extras/ExtensionExample/source/main.d ./TraceRecorder/extras/ExtensionExample/source/main.o ./TraceRecorder/extras/ExtensionExample/source/main.su

.PHONY: clean-TraceRecorder-2f-extras-2f-ExtensionExample-2f-source

