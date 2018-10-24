################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/Fan/fan_hal.c 

OBJS += \
./Sources/Fan/fan_hal.o 

C_DEPS += \
./Sources/Fan/fan_hal.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/Fan/%.o: ../Sources/Fan/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MKL25Z128VLK4" -I"../Sources" -I"../Project_Settings/Startup_Code" -I"../SDK/platform/CMSIS/Include" -I"../SDK/platform/devices" -I"../SDK/platform/devices/MKL25Z4/include" -I"C:\Freescale\KSDK_1.2.0\platform\drivers\inc" -I"C:\Freescale\KSDK_1.2.0\platform\utilities\inc" -I"C:\Freescale\KSDK_1.2.0\platform\hal\inc" -I"C:\Freescale\KSDK_1.2.0\platform\system\inc" -I"C:\Freescale\KSDK_1.2.0\platform\osa\inc" -I"C:\Freescale\KSDK_1.2.0/platform/utilities/inc" -I"C:\Freescale\KSDK_1.2.0/platform/utilities/src" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


