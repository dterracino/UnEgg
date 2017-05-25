################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../nest/third-party/encryption/aes/aescrypt.c \
../nest/third-party/encryption/aes/aeskey.c \
../nest/third-party/encryption/aes/aestab.c \
../nest/third-party/encryption/aes/fileenc.c \
../nest/third-party/encryption/aes/hmac.c \
../nest/third-party/encryption/aes/prng.c \
../nest/third-party/encryption/aes/pwd2key.c \
../nest/third-party/encryption/aes/sha1.c 

OBJS += \
./nest/third-party/encryption/aes/aescrypt.o \
./nest/third-party/encryption/aes/aeskey.o \
./nest/third-party/encryption/aes/aestab.o \
./nest/third-party/encryption/aes/fileenc.o \
./nest/third-party/encryption/aes/hmac.o \
./nest/third-party/encryption/aes/prng.o \
./nest/third-party/encryption/aes/pwd2key.o \
./nest/third-party/encryption/aes/sha1.o 

C_DEPS += \
./nest/third-party/encryption/aes/aescrypt.d \
./nest/third-party/encryption/aes/aeskey.d \
./nest/third-party/encryption/aes/aestab.d \
./nest/third-party/encryption/aes/fileenc.d \
./nest/third-party/encryption/aes/hmac.d \
./nest/third-party/encryption/aes/prng.d \
./nest/third-party/encryption/aes/pwd2key.d \
./nest/third-party/encryption/aes/sha1.d 


# Each subdirectory must supply rules for building sources it contributes
nest/third-party/encryption/aes/%.o: ../nest/third-party/encryption/aes/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -D_7ZIP_ST -O3 -Wall -c -fmessage-length=0 -Wno-unused -Wno-multichar -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


