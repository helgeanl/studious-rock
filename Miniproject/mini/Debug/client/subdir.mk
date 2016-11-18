################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../client/miniproject.c 

OBJS += \
./client/miniproject.o 

C_DEPS += \
./client/miniproject.d 


# Each subdirectory must supply rules for building sources it contributes
client/%.o: ../client/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	avr32-linux-gcc -O0 -g3 -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


