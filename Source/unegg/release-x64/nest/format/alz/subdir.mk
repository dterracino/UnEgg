################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../nest/format/alz/File.cpp \
../nest/format/alz/Global.cpp 

OBJS += \
./nest/format/alz/File.o \
./nest/format/alz/Global.o 

CPP_DEPS += \
./nest/format/alz/File.d \
./nest/format/alz/Global.d 


# Each subdirectory must supply rules for building sources it contributes
nest/format/alz/%.o: ../nest/format/alz/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../. -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


