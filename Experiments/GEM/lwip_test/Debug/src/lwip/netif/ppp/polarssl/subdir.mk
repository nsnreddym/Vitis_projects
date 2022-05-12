################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lwip/netif/ppp/polarssl/arc4.c \
../src/lwip/netif/ppp/polarssl/des.c \
../src/lwip/netif/ppp/polarssl/md4.c \
../src/lwip/netif/ppp/polarssl/md5.c \
../src/lwip/netif/ppp/polarssl/sha1.c 

OBJS += \
./src/lwip/netif/ppp/polarssl/arc4.o \
./src/lwip/netif/ppp/polarssl/des.o \
./src/lwip/netif/ppp/polarssl/md4.o \
./src/lwip/netif/ppp/polarssl/md5.o \
./src/lwip/netif/ppp/polarssl/sha1.o 

C_DEPS += \
./src/lwip/netif/ppp/polarssl/arc4.d \
./src/lwip/netif/ppp/polarssl/des.d \
./src/lwip/netif/ppp/polarssl/md4.d \
./src/lwip/netif/ppp/polarssl/md5.d \
./src/lwip/netif/ppp/polarssl/sha1.d 


# Each subdirectory must supply rules for building sources it contributes
src/lwip/netif/ppp/polarssl/%.o: ../src/lwip/netif/ppp/polarssl/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -IE:/Git_Repositories/Vitis_Workspace/Experiments_V3/Gigabit_Ethernet_Platform/export/Gigabit_Ethernet_Platform/sw/Gigabit_Ethernet_Platform/standalone_domain/bspinclude/include -I"E:\Git_Repositories\Vitis_Workspace\Experiments_V3\lwip_test\src\lwip\include" -I"E:\Git_Repositories\Vitis_Workspace\Experiments_V3\lwip_test\src\lwip\contrib\include" -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


