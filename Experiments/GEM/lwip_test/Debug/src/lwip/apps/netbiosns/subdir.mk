################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lwip/apps/netbiosns/netbiosns.c 

OBJS += \
./src/lwip/apps/netbiosns/netbiosns.o 

C_DEPS += \
./src/lwip/apps/netbiosns/netbiosns.d 


# Each subdirectory must supply rules for building sources it contributes
src/lwip/apps/netbiosns/%.o: ../src/lwip/apps/netbiosns/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -IE:/Git_Repositories/Vitis_projects/Experiments/GEM/Gigabit_Ethernet_Platform/export/Gigabit_Ethernet_Platform/sw/Gigabit_Ethernet_Platform/standalone_domain/bspinclude/include -I"E:\Git_Repositories\Vitis_projects\Experiments\GEM\lwip_test\src\lwip\include" -I"E:\Git_Repositories\Vitis_projects\Experiments\GEM\lwip_test\src\lwip\contrib\include" -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


