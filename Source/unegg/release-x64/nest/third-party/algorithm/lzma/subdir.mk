################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../nest/third-party/algorithm/lzma/7zBuf.c \
../nest/third-party/algorithm/lzma/7zBuf2.c \
../nest/third-party/algorithm/lzma/7zCrc.c \
../nest/third-party/algorithm/lzma/7zFile.c \
../nest/third-party/algorithm/lzma/7zStream.c \
../nest/third-party/algorithm/lzma/Alloc.c \
../nest/third-party/algorithm/lzma/Bcj2.c \
../nest/third-party/algorithm/lzma/Bra.c \
../nest/third-party/algorithm/lzma/Bra86.c \
../nest/third-party/algorithm/lzma/BraIA64.c \
../nest/third-party/algorithm/lzma/CpuArch.c \
../nest/third-party/algorithm/lzma/LzFind.c \
../nest/third-party/algorithm/lzma/LzmaDec.c \
../nest/third-party/algorithm/lzma/LzmaEnc.c 

OBJS += \
./nest/third-party/algorithm/lzma/7zBuf.o \
./nest/third-party/algorithm/lzma/7zBuf2.o \
./nest/third-party/algorithm/lzma/7zCrc.o \
./nest/third-party/algorithm/lzma/7zFile.o \
./nest/third-party/algorithm/lzma/7zStream.o \
./nest/third-party/algorithm/lzma/Alloc.o \
./nest/third-party/algorithm/lzma/Bcj2.o \
./nest/third-party/algorithm/lzma/Bra.o \
./nest/third-party/algorithm/lzma/Bra86.o \
./nest/third-party/algorithm/lzma/BraIA64.o \
./nest/third-party/algorithm/lzma/CpuArch.o \
./nest/third-party/algorithm/lzma/LzFind.o \
./nest/third-party/algorithm/lzma/LzmaDec.o \
./nest/third-party/algorithm/lzma/LzmaEnc.o 

C_DEPS += \
./nest/third-party/algorithm/lzma/7zBuf.d \
./nest/third-party/algorithm/lzma/7zBuf2.d \
./nest/third-party/algorithm/lzma/7zCrc.d \
./nest/third-party/algorithm/lzma/7zFile.d \
./nest/third-party/algorithm/lzma/7zStream.d \
./nest/third-party/algorithm/lzma/Alloc.d \
./nest/third-party/algorithm/lzma/Bcj2.d \
./nest/third-party/algorithm/lzma/Bra.d \
./nest/third-party/algorithm/lzma/Bra86.d \
./nest/third-party/algorithm/lzma/BraIA64.d \
./nest/third-party/algorithm/lzma/CpuArch.d \
./nest/third-party/algorithm/lzma/LzFind.d \
./nest/third-party/algorithm/lzma/LzmaDec.d \
./nest/third-party/algorithm/lzma/LzmaEnc.d 


# Each subdirectory must supply rules for building sources it contributes
nest/third-party/algorithm/lzma/%.o: ../nest/third-party/algorithm/lzma/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -D_7ZIP_ST -O3 -Wall -c -fmessage-length=0 -Wno-unused -Wno-multichar -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


