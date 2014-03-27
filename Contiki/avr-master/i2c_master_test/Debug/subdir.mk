################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../i2c_master_test.c 

OBJS += \
./i2c_master_test.o 

C_DEPS += \
./i2c_master_test.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"/Volumes/DataHDD/Documents/Development Projects/Home_Monitoring_Project/Contiki-Sensor-Node/Contiki/avr-master/i2c" -I"/Volumes/DataHDD/Documents/Development Projects/Home_Monitoring_Project/Contiki-Sensor-Node/Contiki/avr-master/uart" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega644p -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


