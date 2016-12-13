################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/CarCtrl.cpp \
../src/draw_helper.cpp \
../src/img_processing.cpp \
../src/main.cpp \
../src/my_tools.cpp 

OBJS += \
./src/CarCtrl.o \
./src/draw_helper.o \
./src/img_processing.o \
./src/main.o \
./src/my_tools.o 

CPP_DEPS += \
./src/CarCtrl.d \
./src/draw_helper.d \
./src/img_processing.d \
./src/main.d \
./src/my_tools.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/ -O0 -g3 -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


