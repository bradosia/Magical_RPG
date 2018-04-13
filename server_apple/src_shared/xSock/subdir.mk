################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src_shared/xSock/xHTTP.cpp \
../src_shared/xSock/xSock.cpp \
../src_shared/xSock/xWebSock.cpp 

OBJS += \
./src_shared/xSock/xHTTP.o \
./src_shared/xSock/xSock.o \
./src_shared/xSock/xWebSock.o 

CPP_DEPS += \
./src_shared/xSock/xHTTP.d \
./src_shared/xSock/xSock.d \
./src_shared/xSock/xWebSock.d 


# Each subdirectory must supply rules for building sources it contributes
src_shared/xSock/%.o: ../src_shared/xSock/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__cplusplus=201103L -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


