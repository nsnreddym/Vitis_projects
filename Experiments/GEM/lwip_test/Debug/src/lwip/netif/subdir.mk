################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lwip/netif/ethernet.c \
../src/lwip/netif/lowpan6.c \
../src/lwip/netif/lowpan6_ble.c \
../src/lwip/netif/lowpan6_common.c \
../src/lwip/netif/zepif.c 

OBJS += \
./src/lwip/netif/ethernet.o \
./src/lwip/netif/lowpan6.o \
./src/lwip/netif/lowpan6_ble.o \
./src/lwip/netif/lowpan6_common.o \
./src/lwip/netif/zepif.o 

C_DEPS += \
./src/lwip/netif/ethernet.d \
./src/lwip/netif/lowpan6.d \
./src/lwip/netif/lowpan6_ble.d \
./src/lwip/netif/lowpan6_common.d \
./src/lwip/netif/zepif.d 


# Each subdirectory must supply rules for building sources it contributes
src/lwip/netif/%.o: ../src/lwip/netif/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -IE:/Git_Repositories/Vitis_projects/Experiments/GEM/Gigabit_Ethernet_Platform/export/Gigabit_Ethernet_Platform/sw/Gigabit_Ethernet_Platform/standalone_domain/bspinclude/include -I"E:\Git_Repositories\Vitis_projects\Experiments\GEM\lwip_test\src\lwip\include" -I"E:\Git_Repositories\Vitis_projects\Experiments\GEM\lwip_test\src\lwip\contrib\include" -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


