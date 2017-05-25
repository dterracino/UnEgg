################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../nest/format/egg/Block.cpp \
../nest/format/egg/BlockWrap.cpp \
../nest/format/egg/Dummy.cpp \
../nest/format/egg/ExtraField.cpp \
../nest/format/egg/ExtraFieldWrap.cpp \
../nest/format/egg/File.cpp \
../nest/format/egg/FileWrap.cpp \
../nest/format/egg/Global.cpp 

OBJS += \
./nest/format/egg/Block.o \
./nest/format/egg/BlockWrap.o \
./nest/format/egg/Dummy.o \
./nest/format/egg/ExtraField.o \
./nest/format/egg/ExtraFieldWrap.o \
./nest/format/egg/File.o \
./nest/format/egg/FileWrap.o \
./nest/format/egg/Global.o 

CPP_DEPS += \
./nest/format/egg/Block.d \
./nest/format/egg/BlockWrap.d \
./nest/format/egg/Dummy.d \
./nest/format/egg/ExtraField.d \
./nest/format/egg/ExtraFieldWrap.d \
./nest/format/egg/File.d \
./nest/format/egg/FileWrap.d \
./nest/format/egg/Global.d 


# Each subdirectory must supply rules for building sources it contributes
nest/format/egg/%.o: ../nest/format/egg/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../. -O3 -Wall -c -fmessage-length=0 -Wno-unused -Wno-multichar -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


