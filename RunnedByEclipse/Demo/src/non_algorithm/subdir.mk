################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/non_algorithm/draw_helper.cpp 

OBJS += \
./src/non_algorithm/draw_helper.o 

CPP_DEPS += \
./src/non_algorithm/draw_helper.d 


# Each subdirectory must supply rules for building sources it contributes
src/non_algorithm/%.o: ../src/non_algorithm/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/ -O0 -g3 -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


