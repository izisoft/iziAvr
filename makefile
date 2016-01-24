# =================================================================================================
# OS Specificities
ifeq ($(OS),Windows_NT)
	RM := del /F/Q
	PATH_SEP := "\"
	MD := mkdir
else
	RM := rm -rf
	PATH_SEP := /
	MD := mkdir -p
endif

# =================================================================================================
# Helper functions
rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

# =================================================================================================
# Configuration
MCU ?= atmega32
FREQ ?= 7372800
IZI_SYSTEM_TICK_SOURCE ?= 4
IZI_SYSTEM_TICK_RATE ?= 256
IZI_KERNEL_SCHEDULER ?= 0
IZI_KERNEL_SIZE ?= 1

# =================================================================================================
# Names
PROJECT = IziAvr
SRC_DIR = src
BUILD_DIR = build
LIBRARY = $(BUILD_DIR)/lib$(PROJECT).a
LSS = $(BUILD_DIR)/lib$(PROJECT).lss

# =================================================================================================
# Paths
SOURCES += $(call rwildcard, $(SRC_DIR)/, *.c)
DEPS += $(patsubst %.c,$(BUILD_DIR)/%.d, $(SOURCES))
OBJS += $(patsubst %.c,$(BUILD_DIR)/%.o, $(SOURCES))
OUT_DIRS = $(sort $(foreach obj, $(OBJS), $(dir $(obj))))
INCLUDES += -Iinterface -I$(SRC_DIR)

# =================================================================================================
# Compilation Options
WARNINGS = -Wall -Werror
OPTIMIZATION_LEVEL = -Os
FLAGS += $(WARNINGS) $(OPTIMIZATION_LEVEL)
FLAGS += -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -g
DEFS += -mmcu=$(MCU) -DF_CPU=$(FREQ)UL 
DEFS += -DIZI_SYSTEM_TICK_SOURCE=$(IZI_SYSTEM_TICK_SOURCE)
DEFS += -DIZI_SYSTEM_TICK_RATE=$(IZI_SYSTEM_TICK_RATE)
DEFS += -DIZI_KERNEL_SCHEDULER=$(IZI_KERNEL_SCHEDULER)
DEFS += -DIZI_KERNEL_SIZE=$(IZI_KERNEL_SIZE)

# =================================================================================================
# All Target
all: $(OUT_DIRS) $(LIBRARY) $(LSS)
	
# Compilation
build/%.o: %.c
	@echo Building file: $<
	@echo Invoking: AVR Compiler
	avr-gcc $(INCLUDES) $(FLAGS) $(DEFS) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo Finished building: $<
	@echo " "
# Folder structure
build/%:
	$(if $(wildcard $@), , $(MD) $(subst /,$(PATH_SEP),$@))
	
# Library achive
$(LIBRARY): $(OBJS)
	@echo Building target: $@
	@echo Invoking: AVR Archiver
	avr-ar -r $(LIBRARY) $(OBJS) $(LIBS)
	@echo Finished building target: $@
	@echo " " 
	
# Extended listing
$(LSS): $(LIBRARY)
	@echo Invoking: AVR Create Extended Listing
	-avr-objdump -h -S $(LIBRARY)  > $(LSS)
	@echo Finished building: $@
	@echo " "

# Other Targets
clean:
	-$(RM) $(subst /,$(PATH_SEP),$(OBJS) $(DEPS) $(LIBRARY) $(LSS))
	-@echo " " 

secondary-outputs: $(LSS)
	
.PHONY: all clean dependents directories
.SECONDARY:
