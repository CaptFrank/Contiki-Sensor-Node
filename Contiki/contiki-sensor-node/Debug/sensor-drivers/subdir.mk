################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../sensor-drivers/i2c-driver.cpp 

OBJS += \
./sensor-drivers/i2c-driver.o 

CPP_DEPS += \
./sensor-drivers/i2c-driver.d 


# Each subdirectory must supply rules for building sources it contributes
sensor-drivers/%.o: ../sensor-drivers/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"/Volumes/DataHDD/Documents/Development Projects/Home_Monitoring_Project/Contiki-Sensor-Node/Contiki/contiki-sensor-node/sensor-drivers/libds-master" -I"/Volumes/DataHDD/Documents/Development Projects/Home_Monitoring_Project/Contiki-Sensor-Node/Contiki/avrlib" -I"/Volumes/DataHDD/Documents/Development Projects/Home_Monitoring_Project/Contiki-Sensor-Node/Contiki/contiki/apps" -I"/Volumes/DataHDD/Documents/Development Projects/Home_Monitoring_Project/Contiki-Sensor-Node/Contiki/contiki/apps/shell" -I"/Volumes/DataHDD/Documents/Development Projects/Home_Monitoring_Project/Contiki-Sensor-Node/Contiki/contiki/apps/serial-shell" -I"/Volumes/DataHDD/Documents/Development Projects/Home_Monitoring_Project/Contiki-Sensor-Node/Contiki/contiki/core" -I"/Volumes/DataHDD/Documents/Development Projects/Home_Monitoring_Project/Contiki-Sensor-Node/Contiki/Wire" -I"/Volumes/DataHDD/Documents/Development Projects/Home_Monitoring_Project/Contiki-Sensor-Node/Contiki/avr-master" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


