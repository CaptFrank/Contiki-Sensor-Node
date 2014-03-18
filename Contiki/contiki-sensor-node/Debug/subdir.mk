################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../example-shell.c \
../shell-get-data.c \
../shell-i2c-driver.c 

OBJS += \
./example-shell.o \
./shell-get-data.o \
./shell-i2c-driver.o 

C_DEPS += \
./example-shell.d \
./shell-get-data.d \
./shell-i2c-driver.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I/usr/local/CrossPack-AVR-20120217/avr/include -I"/Users/francispapineau/Documents/Coding Projects/Python/Zigbee_Server/Contiki/contiki" -I"/Users/francispapineau/Documents/Coding Projects/Python/Zigbee_Server/Contiki/avrlib" -I"/Users/francispapineau/Documents/Coding Projects/Python/Zigbee_Server/Contiki/contiki/apps" -I"/Users/francispapineau/Documents/Coding Projects/Python/Zigbee_Server/Contiki/contiki/apps/shell" -I"/Users/francispapineau/Documents/Coding Projects/Python/Zigbee_Server/Contiki/contiki/core" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


