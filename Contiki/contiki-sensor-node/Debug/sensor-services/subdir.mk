################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../sensor-services/base-i2c-service.cpp \
../sensor-services/i2c-daemon.cpp 

OBJS += \
./sensor-services/base-i2c-service.o \
./sensor-services/i2c-daemon.o 

CPP_DEPS += \
./sensor-services/base-i2c-service.d \
./sensor-services/i2c-daemon.d 


# Each subdirectory must supply rules for building sources it contributes
sensor-services/%.o: ../sensor-services/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"/Volumes/DataHDD/Documents/Development Projects/Home_Monitoring_Project/Contiki-Sensor-Node/Contiki/contiki-sensor-node/sensor-drivers/libds-master" -I"/Volumes/DataHDD/Documents/Development Projects/Home_Monitoring_Project/Contiki-Sensor-Node/Contiki/avrlib" -I"/Volumes/DataHDD/Documents/Development Projects/Home_Monitoring_Project/Contiki-Sensor-Node/Contiki/contiki/apps" -I"/Volumes/DataHDD/Documents/Development Projects/Home_Monitoring_Project/Contiki-Sensor-Node/Contiki/contiki/apps/shell" -I"/Volumes/DataHDD/Documents/Development Projects/Home_Monitoring_Project/Contiki-Sensor-Node/Contiki/contiki/apps/serial-shell" -I"/Volumes/DataHDD/Documents/Development Projects/Home_Monitoring_Project/Contiki-Sensor-Node/Contiki/contiki/core" -I"/Volumes/DataHDD/Documents/Development Projects/Home_Monitoring_Project/Contiki-Sensor-Node/Contiki/Wire" -I"/Volumes/DataHDD/Documents/Development Projects/Home_Monitoring_Project/Contiki-Sensor-Node/Contiki/avr-master" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


