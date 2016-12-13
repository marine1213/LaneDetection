################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/unsused/thinning.cpp 

C_SRCS += \
../src/unsused/bottomPlaneExtraction.c \
../src/unsused/hough.c 

OBJS += \
./src/unsused/bottomPlaneExtraction.o \
./src/unsused/hough.o \
./src/unsused/thinning.o 

CPP_DEPS += \
./src/unsused/thinning.d 

C_DEPS += \
./src/unsused/bottomPlaneExtraction.d \
./src/unsused/hough.d 


# Each subdirectory must supply rules for building sources it contributes
src/unsused/%.o: ../src/unsused/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/unsused/%.o: ../src/unsused/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/ -O0 -g3 -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


