################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src_shared/imgui/imgui.cpp \
../src_shared/imgui/imgui_demo.cpp \
../src_shared/imgui/imgui_draw.cpp 

OBJS += \
./src_shared/imgui/imgui.o \
./src_shared/imgui/imgui_demo.o \
./src_shared/imgui/imgui_draw.o 

CPP_DEPS += \
./src_shared/imgui/imgui.d \
./src_shared/imgui/imgui_demo.d \
./src_shared/imgui/imgui_draw.d 


# Each subdirectory must supply rules for building sources it contributes
src_shared/imgui/%.o: ../src_shared/imgui/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -std=gnu++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


