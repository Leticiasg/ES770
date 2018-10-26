################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/Util/adc.c \
../Sources/Util/debugUart.c \
../Sources/Util/pid.c \
../Sources/Util/tc_hal.c \
../Sources/Util/timer_counter.c \
../Sources/Util/util.c 

OBJS += \
./Sources/Util/adc.o \
./Sources/Util/debugUart.o \
./Sources/Util/pid.o \
./Sources/Util/tc_hal.o \
./Sources/Util/timer_counter.o \
./Sources/Util/util.o 

C_DEPS += \
./Sources/Util/adc.d \
./Sources/Util/debugUart.d \
./Sources/Util/pid.d \
./Sources/Util/tc_hal.d \
./Sources/Util/timer_counter.d \
./Sources/Util/util.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/Util/%.o: ../Sources/Util/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -D"CPU_MKL25Z128VLK4" -I"../Sources" -I"../Project_Settings/Startup_Code" -I"../SDK/platform/CMSIS/Include" -I"../SDK/platform/devices" -I"../SDK/platform/devices/MKL25Z4/include" -I"C:\Freescale\KSDK_1.2.0\platform\drivers\inc" -I"C:\Freescale\KSDK_1.2.0\platform\utilities\inc" -I"C:\Freescale\KSDK_1.2.0\platform\hal\inc" -I"C:\Freescale\KSDK_1.2.0\platform\system\inc" -I"C:\Freescale\KSDK_1.2.0\platform\osa\inc" -I"C:\Freescale\KSDK_1.2.0/platform/utilities/inc" -I"C:\Freescale\KSDK_1.2.0/platform/utilities/src" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


