################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../nest/third-party/algorithm/bzip2/blocksort.c \
../nest/third-party/algorithm/bzip2/bzlib.c \
../nest/third-party/algorithm/bzip2/compress.c \
../nest/third-party/algorithm/bzip2/crctable.c \
../nest/third-party/algorithm/bzip2/decompress.c \
../nest/third-party/algorithm/bzip2/huffman.c \
../nest/third-party/algorithm/bzip2/randtable.c 

OBJS += \
./nest/third-party/algorithm/bzip2/blocksort.o \
./nest/third-party/algorithm/bzip2/bzlib.o \
./nest/third-party/algorithm/bzip2/compress.o \
./nest/third-party/algorithm/bzip2/crctable.o \
./nest/third-party/algorithm/bzip2/decompress.o \
./nest/third-party/algorithm/bzip2/huffman.o \
./nest/third-party/algorithm/bzip2/randtable.o 

C_DEPS += \
./nest/third-party/algorithm/bzip2/blocksort.d \
./nest/third-party/algorithm/bzip2/bzlib.d \
./nest/third-party/algorithm/bzip2/compress.d \
./nest/third-party/algorithm/bzip2/crctable.d \
./nest/third-party/algorithm/bzip2/decompress.d \
./nest/third-party/algorithm/bzip2/huffman.d \
./nest/third-party/algorithm/bzip2/randtable.d 


# Each subdirectory must supply rules for building sources it contributes
nest/third-party/algorithm/bzip2/%.o: ../nest/third-party/algorithm/bzip2/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -D_7ZIP_ST -O3 -Wall -c -fmessage-length=0 -Wno-unused -Wno-multichar -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


