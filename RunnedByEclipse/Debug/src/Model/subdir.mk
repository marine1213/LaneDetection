################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Model/LineTest.cpp \
../src/Model/testCarCtrl.cpp \
../src/Model/testGoingStraight.cpp \
../src/Model/testSerialCtrl.cpp 

OBJS += \
./src/Model/LineTest.o \
./src/Model/testCarCtrl.o \
./src/Model/testGoingStraight.o \
./src/Model/testSerialCtrl.o 

CPP_DEPS += \
./src/Model/LineTest.d \
./src/Model/testCarCtrl.d \
./src/Model/testGoingStraight.d \
./src/Model/testSerialCtrl.d 


# Each subdirectory must supply rules for building sources it contributes
src/Model/%.o: ../src/Model/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/ -O0 -g3 -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


