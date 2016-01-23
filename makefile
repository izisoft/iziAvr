RM := rm -rf

LSS += libiziAvr.lss

FLASH_IMAGE += iziAvr.hex

EEPROM_IMAGE += iziAvr.eep

SIZEDUMMY += sizedummy

SRC_DIR = src

INCLUDES += -Iinterface -I$(SRC_DIR)

rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

SOURCES += $(call rwildcard, src/, *.c)

DEPS += \ $(patsubst %.c,%.d, $(SOURCES))

OBJS += $(patsubst %.c,%.o, $(SOURCES))

%.o: %.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc $(INCLUDES) -Wall -Werror -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -g -mmcu=atmega32 -DF_CPU=7372800UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

# All Target
all: libiziAvr.a secondary-outputs

# Tool invocations
libiziAvr.a: $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR Archiver'
	avr-ar -r  "libiziAvr.a" $(OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	
libiziAvr.lss: libiziAvr.a
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S libiziAvr.a  >"libiziAvr.lss"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(OBJS)$(C_UPPER_DEPS)$(CXX_DEPS)$(ARCHIVES)$(ASM_DEPS)$(S_DEPS)$(S_UPPER_DEPS)$(LSS)$(C_DEPS)$(CPP_DEPS) libiziAvr.a
	-@echo ' '

secondary-outputs: $(LSS)

.PHONY: all clean dependents
.SECONDARY: