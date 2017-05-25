################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../nest/stream/FileInputStream.cpp \
../nest/stream/FileOutputStream.cpp \
../nest/stream/InputStream.cpp \
../nest/stream/InputStreamLinker.cpp 

OBJS += \
./nest/stream/FileInputStream.o \
./nest/stream/FileOutputStream.o \
./nest/stream/InputStream.o \
./nest/stream/InputStreamLinker.o 

CPP_DEPS += \
./nest/stream/FileInputStream.d \
./nest/stream/FileOutputStream.d \
./nest/stream/InputStream.d \
./nest/stream/InputStreamLinker.d 


# Each subdirectory must supply rules for building sources it contributes
nest/stream/%.o: ../nest/stream/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../. -O3 -Wall -c -fmessage-length=0 -Wno-unused -Wno-multichar -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


