################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../nest/format/ALZFormat.cpp \
../nest/format/ALZFormatFactory.cpp \
../nest/format/EggFormat.cpp \
../nest/format/EggFormatFactory.cpp \
../nest/format/Format.cpp \
../nest/format/Header.cpp 

OBJS += \
./nest/format/ALZFormat.o \
./nest/format/ALZFormatFactory.o \
./nest/format/EggFormat.o \
./nest/format/EggFormatFactory.o \
./nest/format/Format.o \
./nest/format/Header.o 

CPP_DEPS += \
./nest/format/ALZFormat.d \
./nest/format/ALZFormatFactory.d \
./nest/format/EggFormat.d \
./nest/format/EggFormatFactory.d \
./nest/format/Format.d \
./nest/format/Header.d 


# Each subdirectory must supply rules for building sources it contributes
nest/format/%.o: ../nest/format/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../. -O3 -Wall -c -fmessage-length=0 -Wno-unused -Wno-multichar -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


