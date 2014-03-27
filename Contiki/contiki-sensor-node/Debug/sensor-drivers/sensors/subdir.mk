################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../sensor-drivers/sensors/BMP180-driver.cpp \
../sensor-drivers/sensors/EEPROM-driver.cpp \
../sensor-drivers/sensors/TSL2561-driver.cpp \
../sensor-drivers/sensors/fs-driver.cpp 

C_SRCS += \
../sensor-drivers/sensors/HIH6130-driver.c 

OBJS += \
./sensor-drivers/sensors/BMP180-driver.o \
./sensor-drivers/sensors/EEPROM-driver.o \
./sensor-drivers/sensors/HIH6130-driver.o \
./sensor-drivers/sensors/TSL2561-driver.o \
./sensor-drivers/sensors/fs-driver.o 

C_DEPS += \
./sensor-drivers/sensors/HIH6130-driver.d 

CPP_DEPS += \
./sensor-drivers/sensors/BMP180-driver.d \
./sensor-drivers/sensors/EEPROM-driver.d \
./sensor-drivers/sensors/TSL2561-driver.d \
./sensor-drivers/sensors/fs-driver.d 


# Each subdirectory must supply rules for building sources it contributes
sensor-drivers/sensors/%.o: ../sensor-drivers/sensors/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"/Volumes/DataHDD/Documents/Development Projects/Home_Monitoring_Project/Contiki-Sensor-Node/Contiki/contiki-sensor-node/sensor-drivers/libds-master" -I"/Volumes/DataHDD/Documents/Development Projects/Home_Monitoring_Project/Contiki-Sensor-Node/Contiki/avrlib" -I"/Volumes/DataHDD/Documents/Development Projects/Home_Monitoring_Project/Contiki-Sensor-Node/Contiki/contiki/apps" -I"/Volumes/DataHDD/Documents/Development Projects/Home_Monitoring_Project/Contiki-Sensor-Node/Contiki/contiki/apps/shell" -I"/Volumes/DataHDD/Documents/Development Projects/Home_Monitoring_Project/Contiki-Sensor-Node/Contiki/contiki/apps/serial-shell" -I"/Volumes/DataHDD/Documents/Development Projects/Home_Monitoring_Project/Contiki-Sensor-Node/Contiki/contiki/core" -I"/Volumes/DataHDD/Documents/Development Projects/Home_Monitoring_Project/Contiki-Sensor-Node/Contiki/Wire" -I"/Volumes/DataHDD/Documents/Development Projects/Home_Monitoring_Project/Contiki-Sensor-Node/Contiki/avr-master" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

sensor-drivers/sensors/%.o: ../sensor-drivers/sensors/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"/Volumes/DataHDD/Documents/Development Projects/Home_Monitoring_Project/Contiki-Sensor-Node/Contiki/contiki-sensor-node/sensor-drivers/libds-master" -I"/Volumes/DataHDD/Documents/Development Projects/Home_Monitoring_Project/Contiki-Sensor-Node/Contiki/contiki/apps" -I"/Volumes/DataHDD/Documents/Development Projects/Home_Monitoring_Project/Contiki-Sensor-Node/Contiki/contiki/apps/shell" -I"/Volumes/DataHDD/Documents/Development Projects/Home_Monitoring_Project/Contiki-Sensor-Node/Contiki/contiki/apps/serial-shell" -I"/Volumes/DataHDD/Documents/Development Projects/Home_Monitoring_Project/Contiki-Sensor-Node/Contiki/contiki/core" -I"/Volumes/DataHDD/Documents/Development Projects/Home_Monitoring_Project/Contiki-Sensor-Node/Contiki/Wire" -I"/Volumes/DataHDD/Documents/Development Projects/Home_Monitoring_Project/Contiki-Sensor-Node/Contiki/avr-master" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


