################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HMI_ECU.c \
../buzzer.c \
../external_eeprom.c \
../gpio.c \
../keypad.c \
../lcd.c \
../motor.c \
../pwm.c \
../timer.c \
../twi.c \
../uart.c 

OBJS += \
./HMI_ECU.o \
./buzzer.o \
./external_eeprom.o \
./gpio.o \
./keypad.o \
./lcd.o \
./motor.o \
./pwm.o \
./timer.o \
./twi.o \
./uart.o 

C_DEPS += \
./HMI_ECU.d \
./buzzer.d \
./external_eeprom.d \
./gpio.d \
./keypad.d \
./lcd.d \
./motor.d \
./pwm.d \
./timer.d \
./twi.d \
./uart.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


