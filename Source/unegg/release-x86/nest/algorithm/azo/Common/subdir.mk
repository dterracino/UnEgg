################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../nest/algorithm/azo/Common/Allocator.cpp \
../nest/algorithm/azo/Common/BufferMan.cpp \
../nest/algorithm/azo/Common/ProbCount.cpp \
../nest/algorithm/azo/Common/x86Filter.cpp 

OBJS += \
./nest/algorithm/azo/Common/Allocator.o \
./nest/algorithm/azo/Common/BufferMan.o \
./nest/algorithm/azo/Common/ProbCount.o \
./nest/algorithm/azo/Common/x86Filter.o 

CPP_DEPS += \
./nest/algorithm/azo/Common/Allocator.d \
./nest/algorithm/azo/Common/BufferMan.d \
./nest/algorithm/azo/Common/ProbCount.d \
./nest/algorithm/azo/Common/x86Filter.d 


# Each subdirectory must supply rules for building sources it contributes
nest/algorithm/azo/Common/%.o: ../nest/algorithm/azo/Common/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../. -O3 -Wall -c -fmessage-length=0 -Wno-unused -Wno-multichar -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


