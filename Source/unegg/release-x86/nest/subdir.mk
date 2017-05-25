################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../nest/Info.cpp \
../nest/Utils.cpp \
../nest/Workspace.cpp 

OBJS += \
./nest/Info.o \
./nest/Utils.o \
./nest/Workspace.o 

CPP_DEPS += \
./nest/Info.d \
./nest/Utils.d \
./nest/Workspace.d 


# Each subdirectory must supply rules for building sources it contributes
nest/%.o: ../nest/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../. -O3 -Wall -c -fmessage-length=0 -Wno-unused -Wno-multichar -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


