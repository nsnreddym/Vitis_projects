################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lwip/core/ipv6/dhcp6.c \
../src/lwip/core/ipv6/ethip6.c \
../src/lwip/core/ipv6/icmp6.c \
../src/lwip/core/ipv6/inet6.c \
../src/lwip/core/ipv6/ip6.c \
../src/lwip/core/ipv6/ip6_addr.c \
../src/lwip/core/ipv6/ip6_frag.c \
../src/lwip/core/ipv6/mld6.c \
../src/lwip/core/ipv6/nd6.c 

OBJS += \
./src/lwip/core/ipv6/dhcp6.o \
./src/lwip/core/ipv6/ethip6.o \
./src/lwip/core/ipv6/icmp6.o \
./src/lwip/core/ipv6/inet6.o \
./src/lwip/core/ipv6/ip6.o \
./src/lwip/core/ipv6/ip6_addr.o \
./src/lwip/core/ipv6/ip6_frag.o \
./src/lwip/core/ipv6/mld6.o \
./src/lwip/core/ipv6/nd6.o 

C_DEPS += \
./src/lwip/core/ipv6/dhcp6.d \
./src/lwip/core/ipv6/ethip6.d \
./src/lwip/core/ipv6/icmp6.d \
./src/lwip/core/ipv6/inet6.d \
./src/lwip/core/ipv6/ip6.d \
./src/lwip/core/ipv6/ip6_addr.d \
./src/lwip/core/ipv6/ip6_frag.d \
./src/lwip/core/ipv6/mld6.d \
./src/lwip/core/ipv6/nd6.d 


# Each subdirectory must supply rules for building sources it contributes
src/lwip/core/ipv6/%.o: ../src/lwip/core/ipv6/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -IE:/Git_Repositories/Vitis_projects/Experiments/GEM/Gigabit_Ethernet_Platform/export/Gigabit_Ethernet_Platform/sw/Gigabit_Ethernet_Platform/standalone_domain/bspinclude/include -I"E:\Git_Repositories\Vitis_projects\Experiments\GEM\lwip_test\src\lwip\include" -I"E:\Git_Repositories\Vitis_projects\Experiments\GEM\lwip_test\src\lwip\contrib\include" -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


