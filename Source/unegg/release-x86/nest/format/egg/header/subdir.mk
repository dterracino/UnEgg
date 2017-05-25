################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../nest/format/egg/header/BaseHeader.cpp \
../nest/format/egg/header/BlockHeader.cpp \
../nest/format/egg/header/DummyHeader.cpp \
../nest/format/egg/header/ExtraField.cpp \
../nest/format/egg/header/ExtraFields.cpp \
../nest/format/egg/header/FileHeader.cpp \
../nest/format/egg/header/GlobalHeader.cpp \
../nest/format/egg/header/IndividualBlock.cpp \
../nest/format/egg/header/IndividualFile.cpp 

OBJS += \
./nest/format/egg/header/BaseHeader.o \
./nest/format/egg/header/BlockHeader.o \
./nest/format/egg/header/DummyHeader.o \
./nest/format/egg/header/ExtraField.o \
./nest/format/egg/header/ExtraFields.o \
./nest/format/egg/header/FileHeader.o \
./nest/format/egg/header/GlobalHeader.o \
./nest/format/egg/header/IndividualBlock.o \
./nest/format/egg/header/IndividualFile.o 

CPP_DEPS += \
./nest/format/egg/header/BaseHeader.d \
./nest/format/egg/header/BlockHeader.d \
./nest/format/egg/header/DummyHeader.d \
./nest/format/egg/header/ExtraField.d \
./nest/format/egg/header/ExtraFields.d \
./nest/format/egg/header/FileHeader.d \
./nest/format/egg/header/GlobalHeader.d \
./nest/format/egg/header/IndividualBlock.d \
./nest/format/egg/header/IndividualFile.d 


# Each subdirectory must supply rules for building sources it contributes
nest/format/egg/header/%.o: ../nest/format/egg/header/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../. -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


