################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../nest/algorithm/azo/Decoder/AlphaCodeD.cpp \
../nest/algorithm/azo/Decoder/BitCodeD.cpp \
../nest/algorithm/azo/Decoder/BlockCodeD.cpp \
../nest/algorithm/azo/Decoder/BoolStateD.cpp \
../nest/algorithm/azo/Decoder/DictionaryTableD.cpp \
../nest/algorithm/azo/Decoder/DistanceCodeD.cpp \
../nest/algorithm/azo/Decoder/EntropyBitProbD.cpp \
../nest/algorithm/azo/Decoder/EntropyCodeD.cpp \
../nest/algorithm/azo/Decoder/HistoryListD.cpp \
../nest/algorithm/azo/Decoder/LengthCodeD.cpp \
../nest/algorithm/azo/Decoder/MainCodeD.cpp \
../nest/algorithm/azo/Decoder/MatchCodeD.cpp \
../nest/algorithm/azo/Decoder/PredictProbD.cpp \
../nest/algorithm/azo/Decoder/SymbolCodeD.cpp 

OBJS += \
./nest/algorithm/azo/Decoder/AlphaCodeD.o \
./nest/algorithm/azo/Decoder/BitCodeD.o \
./nest/algorithm/azo/Decoder/BlockCodeD.o \
./nest/algorithm/azo/Decoder/BoolStateD.o \
./nest/algorithm/azo/Decoder/DictionaryTableD.o \
./nest/algorithm/azo/Decoder/DistanceCodeD.o \
./nest/algorithm/azo/Decoder/EntropyBitProbD.o \
./nest/algorithm/azo/Decoder/EntropyCodeD.o \
./nest/algorithm/azo/Decoder/HistoryListD.o \
./nest/algorithm/azo/Decoder/LengthCodeD.o \
./nest/algorithm/azo/Decoder/MainCodeD.o \
./nest/algorithm/azo/Decoder/MatchCodeD.o \
./nest/algorithm/azo/Decoder/PredictProbD.o \
./nest/algorithm/azo/Decoder/SymbolCodeD.o 

CPP_DEPS += \
./nest/algorithm/azo/Decoder/AlphaCodeD.d \
./nest/algorithm/azo/Decoder/BitCodeD.d \
./nest/algorithm/azo/Decoder/BlockCodeD.d \
./nest/algorithm/azo/Decoder/BoolStateD.d \
./nest/algorithm/azo/Decoder/DictionaryTableD.d \
./nest/algorithm/azo/Decoder/DistanceCodeD.d \
./nest/algorithm/azo/Decoder/EntropyBitProbD.d \
./nest/algorithm/azo/Decoder/EntropyCodeD.d \
./nest/algorithm/azo/Decoder/HistoryListD.d \
./nest/algorithm/azo/Decoder/LengthCodeD.d \
./nest/algorithm/azo/Decoder/MainCodeD.d \
./nest/algorithm/azo/Decoder/MatchCodeD.d \
./nest/algorithm/azo/Decoder/PredictProbD.d \
./nest/algorithm/azo/Decoder/SymbolCodeD.d 


# Each subdirectory must supply rules for building sources it contributes
nest/algorithm/azo/Decoder/%.o: ../nest/algorithm/azo/Decoder/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../. -O3 -Wall -c -fmessage-length=0 -Wno-unused -Wno-multichar -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


