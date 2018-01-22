################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src_shared/POSIX_HTTP.cpp \
../src_shared/POSIX_SOCKET.cpp \
../src_shared/U_HTTP.cpp \
../src_shared/U_SOCKET.cpp \
../src_shared/WIN_HTTP.cpp \
../src_shared/WIN_SOCKET.cpp 

OBJS += \
./src_shared/POSIX_HTTP.o \
./src_shared/POSIX_SOCKET.o \
./src_shared/U_HTTP.o \
./src_shared/U_SOCKET.o \
./src_shared/WIN_HTTP.o \
./src_shared/WIN_SOCKET.o 

CPP_DEPS += \
./src_shared/POSIX_HTTP.d \
./src_shared/POSIX_SOCKET.d \
./src_shared/U_HTTP.d \
./src_shared/U_SOCKET.d \
./src_shared/WIN_HTTP.d \
./src_shared/WIN_SOCKET.d 


# Each subdirectory must supply rules for building sources it contributes
src_shared/%.o: ../src_shared/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


