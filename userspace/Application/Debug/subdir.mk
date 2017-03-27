################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../computeTask.c \
../main.c \
../spiTask.c \
../tcpClientTask.c \
../tcpServerTask.c \
../uartTask1.c \
../uartTask2.c \
../utilities.c \
../writeDataTask.c 

OBJS += \
./computeTask.o \
./main.o \
./spiTask.o \
./tcpClientTask.o \
./tcpServerTask.o \
./uartTask1.o \
./uartTask2.o \
./utilities.o \
./writeDataTask.o 

C_DEPS += \
./computeTask.d \
./main.d \
./spiTask.d \
./tcpClientTask.d \
./tcpServerTask.d \
./uartTask1.d \
./uartTask2.d \
./utilities.d \
./writeDataTask.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


