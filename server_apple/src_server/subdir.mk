################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src_server/MR_logic.cpp \
../src_server/main.cpp 

OBJS += \
./src_server/MR_logic.o \
./src_server/main.o 

CPP_DEPS += \
./src_server/MR_logic.d \
./src_server/main.d 


# Each subdirectory must supply rules for building sources it contributes
src_server/%.o: ../src_server/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__cplusplus=201103L -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


