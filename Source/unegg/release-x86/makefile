################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include nest/third-party/encryption/aes/subdir.mk
-include nest/third-party/algorithm/zlib/subdir.mk
-include nest/third-party/algorithm/lzma/subdir.mk
-include nest/third-party/algorithm/bzip2/subdir.mk
-include nest/stream/subdir.mk
-include nest/format/egg/subdir.mk
-include nest/format/alz/subdir.mk
-include nest/format/subdir.mk
-include nest/encryption/subdir.mk
-include nest/algorithm/azo/subdir.mk
-include nest/algorithm/azo/Decoder/subdir.mk
-include nest/algorithm/azo/Common/subdir.mk
-include nest/algorithm/azo/Base/subdir.mk
-include nest/algorithm/subdir.mk
-include nest/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: unegg

# Tool invocations
unegg: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++  -o  "unegg" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) unegg
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
