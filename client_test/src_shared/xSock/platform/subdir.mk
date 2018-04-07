################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src_shared/xSock/platform/APPLE_HTTP.cpp \
../src_shared/xSock/platform/APPLE_SOCKET.cpp \
../src_shared/xSock/platform/POSIX_HTTP.cpp \
../src_shared/xSock/platform/POSIX_SOCKET.cpp \
../src_shared/xSock/platform/StringHelper.cpp \
../src_shared/xSock/platform/WIN_HTTP.cpp \
../src_shared/xSock/platform/WIN_SOCKET.cpp 

OBJS += \
./src_shared/xSock/platform/APPLE_HTTP.o \
./src_shared/xSock/platform/APPLE_SOCKET.o \
./src_shared/xSock/platform/POSIX_HTTP.o \
./src_shared/xSock/platform/POSIX_SOCKET.o \
./src_shared/xSock/platform/StringHelper.o \
./src_shared/xSock/platform/WIN_HTTP.o \
./src_shared/xSock/platform/WIN_SOCKET.o 

CPP_DEPS += \
./src_shared/xSock/platform/APPLE_HTTP.d \
./src_shared/xSock/platform/APPLE_SOCKET.d \
./src_shared/xSock/platform/POSIX_HTTP.d \
./src_shared/xSock/platform/POSIX_SOCKET.d \
./src_shared/xSock/platform/StringHelper.d \
./src_shared/xSock/platform/WIN_HTTP.d \
./src_shared/xSock/platform/WIN_SOCKET.d 


# Each subdirectory must supply rules for building sources it contributes
src_shared/xSock/platform/%.o: ../src_shared/xSock/platform/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -std=gnu++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


