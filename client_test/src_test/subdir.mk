################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src_test/imgui_impl_glfw_gl2.cpp \
../src_test/main.cpp 

OBJS += \
./src_test/imgui_impl_glfw_gl2.o \
./src_test/main.o 

CPP_DEPS += \
./src_test/imgui_impl_glfw_gl2.d \
./src_test/main.d 


# Each subdirectory must supply rules for building sources it contributes
src_test/%.o: ../src_test/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -std=gnu++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


