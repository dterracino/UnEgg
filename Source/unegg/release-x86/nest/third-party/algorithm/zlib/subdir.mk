################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../nest/third-party/algorithm/zlib/adler32.c \
../nest/third-party/algorithm/zlib/compress.c \
../nest/third-party/algorithm/zlib/crc32.c \
../nest/third-party/algorithm/zlib/deflate.c \
../nest/third-party/algorithm/zlib/gzclose.c \
../nest/third-party/algorithm/zlib/gzlib.c \
../nest/third-party/algorithm/zlib/gzread.c \
../nest/third-party/algorithm/zlib/gzwrite.c \
../nest/third-party/algorithm/zlib/infback.c \
../nest/third-party/algorithm/zlib/inffast.c \
../nest/third-party/algorithm/zlib/inflate.c \
../nest/third-party/algorithm/zlib/inftrees.c \
../nest/third-party/algorithm/zlib/trees.c \
../nest/third-party/algorithm/zlib/uncompr.c \
../nest/third-party/algorithm/zlib/zutil.c 

OBJS += \
./nest/third-party/algorithm/zlib/adler32.o \
./nest/third-party/algorithm/zlib/compress.o \
./nest/third-party/algorithm/zlib/crc32.o \
./nest/third-party/algorithm/zlib/deflate.o \
./nest/third-party/algorithm/zlib/gzclose.o \
./nest/third-party/algorithm/zlib/gzlib.o \
./nest/third-party/algorithm/zlib/gzread.o \
./nest/third-party/algorithm/zlib/gzwrite.o \
./nest/third-party/algorithm/zlib/infback.o \
./nest/third-party/algorithm/zlib/inffast.o \
./nest/third-party/algorithm/zlib/inflate.o \
./nest/third-party/algorithm/zlib/inftrees.o \
./nest/third-party/algorithm/zlib/trees.o \
./nest/third-party/algorithm/zlib/uncompr.o \
./nest/third-party/algorithm/zlib/zutil.o 

C_DEPS += \
./nest/third-party/algorithm/zlib/adler32.d \
./nest/third-party/algorithm/zlib/compress.d \
./nest/third-party/algorithm/zlib/crc32.d \
./nest/third-party/algorithm/zlib/deflate.d \
./nest/third-party/algorithm/zlib/gzclose.d \
./nest/third-party/algorithm/zlib/gzlib.d \
./nest/third-party/algorithm/zlib/gzread.d \
./nest/third-party/algorithm/zlib/gzwrite.d \
./nest/third-party/algorithm/zlib/infback.d \
./nest/third-party/algorithm/zlib/inffast.d \
./nest/third-party/algorithm/zlib/inflate.d \
./nest/third-party/algorithm/zlib/inftrees.d \
./nest/third-party/algorithm/zlib/trees.d \
./nest/third-party/algorithm/zlib/uncompr.d \
./nest/third-party/algorithm/zlib/zutil.d 


# Each subdirectory must supply rules for building sources it contributes
nest/third-party/algorithm/zlib/%.o: ../nest/third-party/algorithm/zlib/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -D_7ZIP_ST -O3 -Wall -c -fmessage-length=0 -Wno-unused -Wno-multichar -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


