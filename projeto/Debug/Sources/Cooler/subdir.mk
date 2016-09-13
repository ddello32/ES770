################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/Cooler/cooler_hal.c 

OBJS += \
./Sources/Cooler/cooler_hal.o 

C_DEPS += \
./Sources/Cooler/cooler_hal.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/Cooler/%.o: ../Sources/Cooler/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -D"CPU_MKL25Z128VLK4" -I"../Sources" -I"/usb/adapter/sources/sdk" -I"/usb/adapter/sources/" -I"/usb/usb_core/device/sources/classes/include" -I"../Project_Settings/Startup_Code" -I"../SDK/platform/CMSIS/Include" -I"../SDK/platform/devices" -I"../SDK/platform/devices/MKL25Z4/include" -I/platform/utilities/inc -I/platform/hal/inc -I"/platform/system/inc" -I"/platform/drivers/inc" -I"/usb/usb_core/device/include/" -I"/usb/usb_core/include" -I"/usb/usb_core/device/include/MKL25Z4" -I"/usb/usb_core/device/lib/bm/kds/MKL25Z4" -I"/platform/utilities/inc" -I"/platform/utilities/src" -I"/platform/osa/inc" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


