################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../nest/algorithm/azo/unAZO.cpp 

OBJS += \
./nest/algorithm/azo/unAZO.o 

CPP_DEPS += \
./nest/algorithm/azo/unAZO.d 


# Each subdirectory must supply rules for building sources it contributes
nest/algorithm/azo/%.o: ../nest/algorithm/azo/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../. -O3 -Wall -c -fmessage-length=0 -Wno-unused -Wno-multichar -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


