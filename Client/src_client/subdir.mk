################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src_client/AudioLibBass.cpp \
../src_client/cArrow.cpp \
../src_client/cBicho.cpp \
../src_client/cData.cpp \
../src_client/cEnemy.cpp \
../src_client/cFireball.cpp \
../src_client/cGame.cpp \
../src_client/cMonster1.cpp \
../src_client/cMonster2.cpp \
../src_client/cOverlay.cpp \
../src_client/cPlayer.cpp \
../src_client/cPlayer1.cpp \
../src_client/cPlayer2.cpp \
../src_client/cProjectile.cpp \
../src_client/cScene.cpp \
../src_client/cShader.cpp \
../src_client/cSound.cpp \
../src_client/cTexture.cpp \
../src_client/cToken.cpp \
../src_client/main.cpp 

OBJS += \
./src_client/AudioLibBass.o \
./src_client/cArrow.o \
./src_client/cBicho.o \
./src_client/cData.o \
./src_client/cEnemy.o \
./src_client/cFireball.o \
./src_client/cGame.o \
./src_client/cMonster1.o \
./src_client/cMonster2.o \
./src_client/cOverlay.o \
./src_client/cPlayer.o \
./src_client/cPlayer1.o \
./src_client/cPlayer2.o \
./src_client/cProjectile.o \
./src_client/cScene.o \
./src_client/cShader.o \
./src_client/cSound.o \
./src_client/cTexture.o \
./src_client/cToken.o \
./src_client/main.o 

CPP_DEPS += \
./src_client/AudioLibBass.d \
./src_client/cArrow.d \
./src_client/cBicho.d \
./src_client/cData.d \
./src_client/cEnemy.d \
./src_client/cFireball.d \
./src_client/cGame.d \
./src_client/cMonster1.d \
./src_client/cMonster2.d \
./src_client/cOverlay.d \
./src_client/cPlayer.d \
./src_client/cPlayer1.d \
./src_client/cPlayer2.d \
./src_client/cProjectile.d \
./src_client/cScene.d \
./src_client/cShader.d \
./src_client/cSound.d \
./src_client/cTexture.d \
./src_client/cToken.d \
./src_client/main.d 


# Each subdirectory must supply rules for building sources it contributes
src_client/%.o: ../src_client/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


