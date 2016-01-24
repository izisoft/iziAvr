ifeq ($(OS),Windows_NT)
RM := del /F/Q
PATH_SEP := "\"
MD := mkdir
else
RM := rm -rf
PATH_SEP := /
MD := mkdir -p
endif

LSS += libiziAvr.lss

FLASH_IMAGE += iziAvr.hex

EEPROM_IMAGE += iziAvr.eep

SIZEDUMMY += sizedummy

SRC_DIR = src

INCLUDES += -Iinterface -I$(SRC_DIR)

rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

SOURCES += $(call rwildcard, src/, *.c)

DEPS += $(patsubst %.c,build/%.d, $(SOURCES))

OBJS += $(patsubst %.c,build/%.o, $(SOURCES))

OUT_DIRS = $(sort $(foreach obj, $(OBJS), $(dir $(obj))))
	
build/%.o: %.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler;
	avr-gcc $(INCLUDES) -Wall -Werror -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -g -mmcu=atmega32 -DF_CPU=7372800UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
	
build/%:
	$(if $(wildcard $@), , $(MD) $(subst /,$(PATH_SEP),$@))

# All Target
all: $(OUT_DIRS) libiziAvr.a secondary-outputs

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
	-$(RM) $(subst /,$(PATH_SEP),$(OBJS)) $(subst /,$(PATH_SEP),$(DEPS)) $(LSS) libiziAvr.a
	-@echo ' '

secondary-outputs: $(LSS)
	
.PHONY: all clean dependents directories
.SECONDARY:
