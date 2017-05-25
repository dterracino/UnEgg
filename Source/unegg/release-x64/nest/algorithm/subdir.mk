################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../nest/algorithm/AZOCoder.cpp \
../nest/algorithm/BZipCoder.cpp \
../nest/algorithm/Coder.cpp \
../nest/algorithm/DeflateCoder.cpp \
../nest/algorithm/LZMACoder.cpp \
../nest/algorithm/StoreCoder.cpp 

OBJS += \
./nest/algorithm/AZOCoder.o \
./nest/algorithm/BZipCoder.o \
./nest/algorithm/Coder.o \
./nest/algorithm/DeflateCoder.o \
./nest/algorithm/LZMACoder.o \
./nest/algorithm/StoreCoder.o 

CPP_DEPS += \
./nest/algorithm/AZOCoder.d \
./nest/algorithm/BZipCoder.d \
./nest/algorithm/Coder.d \
./nest/algorithm/DeflateCoder.d \
./nest/algorithm/LZMACoder.d \
./nest/algorithm/StoreCoder.d 


# Each subdirectory must supply rules for building sources it contributes
nest/algorithm/%.o: ../nest/algorithm/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../. -O3 -Wall -c -fmessage-length=0 -Wno-unused -Wno-multichar -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


