################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../robot_platform_mini.c 

OBJS += \
./robot_platform_mini.o 

C_DEPS += \
./robot_platform_mini.d 


# Each subdirectory must supply rules for building sources it contributes
robot_platform_mini.o: ../robot_platform_mini.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"/Volumes/DataHDD/Documents/Development Projects/Home_Monitoring_Project/Contiki-Sensor-Node/Contiki/avr-master/uart" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -Wl,-u,vfprintf -mmcu=atmega644 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"robot_platform_mini.d" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


