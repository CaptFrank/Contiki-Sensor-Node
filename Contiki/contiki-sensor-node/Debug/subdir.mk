################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../shell-get-data.c \
../shell-node.c 

OBJS += \
./shell-get-data.o \
./shell-node.o 

C_DEPS += \
./shell-get-data.d \
./shell-node.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"/Volumes/DataHDD/Documents/Development Projects/Home_Monitoring_Project/Contiki-Sensor-Node/Contiki/contiki-sensor-node/sensor-drivers/libds-master" -I"/Volumes/DataHDD/Documents/Development Projects/Home_Monitoring_Project/Contiki-Sensor-Node/Contiki/contiki/apps" -I"/Volumes/DataHDD/Documents/Development Projects/Home_Monitoring_Project/Contiki-Sensor-Node/Contiki/contiki/apps/shell" -I"/Volumes/DataHDD/Documents/Development Projects/Home_Monitoring_Project/Contiki-Sensor-Node/Contiki/contiki/apps/serial-shell" -I"/Volumes/DataHDD/Documents/Development Projects/Home_Monitoring_Project/Contiki-Sensor-Node/Contiki/contiki/core" -I"/Volumes/DataHDD/Documents/Development Projects/Home_Monitoring_Project/Contiki-Sensor-Node/Contiki/Wire" -I"/Volumes/DataHDD/Documents/Development Projects/Home_Monitoring_Project/Contiki-Sensor-Node/Contiki/avr-master" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


