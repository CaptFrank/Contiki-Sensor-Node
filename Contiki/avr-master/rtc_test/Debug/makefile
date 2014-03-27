################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
rtc_test.lss \

FLASH_IMAGE += \
rtc_test.hex \

SIZEDUMMY += \
sizedummy \


# All Target
all: rtc_test.elf secondary-outputs

# Tool invocations
rtc_test.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,rtc_test.map -mmcu=atmega644 -o "rtc_test.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

rtc_test.lss: rtc_test.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S rtc_test.elf  >"rtc_test.lss"
	@echo 'Finished building: $@'
	@echo ' '

rtc_test.hex: rtc_test.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex rtc_test.elf  "rtc_test.hex"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: rtc_test.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega644 rtc_test.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(FLASH_IMAGE)$(ELFS)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) rtc_test.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
