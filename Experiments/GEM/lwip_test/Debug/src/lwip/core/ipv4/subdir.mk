################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lwip/core/ipv4/autoip.c \
../src/lwip/core/ipv4/dhcp.c \
../src/lwip/core/ipv4/etharp.c \
../src/lwip/core/ipv4/icmp.c \
../src/lwip/core/ipv4/igmp.c \
../src/lwip/core/ipv4/ip4.c \
../src/lwip/core/ipv4/ip4_addr.c \
../src/lwip/core/ipv4/ip4_frag.c 

OBJS += \
./src/lwip/core/ipv4/autoip.o \
./src/lwip/core/ipv4/dhcp.o \
./src/lwip/core/ipv4/etharp.o \
./src/lwip/core/ipv4/icmp.o \
./src/lwip/core/ipv4/igmp.o \
./src/lwip/core/ipv4/ip4.o \
./src/lwip/core/ipv4/ip4_addr.o \
./src/lwip/core/ipv4/ip4_frag.o 

C_DEPS += \
./src/lwip/core/ipv4/autoip.d \
./src/lwip/core/ipv4/dhcp.d \
./src/lwip/core/ipv4/etharp.d \
./src/lwip/core/ipv4/icmp.d \
./src/lwip/core/ipv4/igmp.d \
./src/lwip/core/ipv4/ip4.d \
./src/lwip/core/ipv4/ip4_addr.d \
./src/lwip/core/ipv4/ip4_frag.d 


# Each subdirectory must supply rules for building sources it contributes
src/lwip/core/ipv4/%.o: ../src/lwip/core/ipv4/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -IE:/Git_Repositories/Vitis_Workspace/Experiments_V3/Gigabit_Ethernet_Platform/export/Gigabit_Ethernet_Platform/sw/Gigabit_Ethernet_Platform/standalone_domain/bspinclude/include -I"E:\Git_Repositories\Vitis_Workspace\Experiments_V3\lwip_test\src\lwip\include" -I"E:\Git_Repositories\Vitis_Workspace\Experiments_V3\lwip_test\src\lwip\contrib\include" -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


