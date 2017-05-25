################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../nest/algorithm/azo/Base/BoolStateB.cpp \
../nest/algorithm/azo/Base/DictionaryTableB.cpp \
../nest/algorithm/azo/Base/EntropyBitProbB.cpp \
../nest/algorithm/azo/Base/EntropyCodeB.cpp \
../nest/algorithm/azo/Base/HistoryListB.cpp \
../nest/algorithm/azo/Base/MainCodeB.cpp \
../nest/algorithm/azo/Base/PredictProbB.cpp 

OBJS += \
./nest/algorithm/azo/Base/BoolStateB.o \
./nest/algorithm/azo/Base/DictionaryTableB.o \
./nest/algorithm/azo/Base/EntropyBitProbB.o \
./nest/algorithm/azo/Base/EntropyCodeB.o \
./nest/algorithm/azo/Base/HistoryListB.o \
./nest/algorithm/azo/Base/MainCodeB.o \
./nest/algorithm/azo/Base/PredictProbB.o 

CPP_DEPS += \
./nest/algorithm/azo/Base/BoolStateB.d \
./nest/algorithm/azo/Base/DictionaryTableB.d \
./nest/algorithm/azo/Base/EntropyBitProbB.d \
./nest/algorithm/azo/Base/EntropyCodeB.d \
./nest/algorithm/azo/Base/HistoryListB.d \
./nest/algorithm/azo/Base/MainCodeB.d \
./nest/algorithm/azo/Base/PredictProbB.d 


# Each subdirectory must supply rules for building sources it contributes
nest/algorithm/azo/Base/%.o: ../nest/algorithm/azo/Base/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../. -O3 -Wall -c -fmessage-length=0 -Wno-unused -Wno-multichar -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


