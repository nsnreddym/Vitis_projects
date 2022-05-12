################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lwip/apps/tftp/tftp_server.c 

OBJS += \
./src/lwip/apps/tftp/tftp_server.o 

C_DEPS += \
./src/lwip/apps/tftp/tftp_server.d 


# Each subdirectory must supply rules for building sources it contributes
src/lwip/apps/tftp/%.o: ../src/lwip/apps/tftp/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -IE:/Git_Repositories/Vitis_Workspace/Experiments_V3/Gigabit_Ethernet_Platform/export/Gigabit_Ethernet_Platform/sw/Gigabit_Ethernet_Platform/standalone_domain/bspinclude/include -I"E:\Git_Repositories\Vitis_Workspace\Experiments_V3\lwip_test\src\lwip\include" -I"E:\Git_Repositories\Vitis_Workspace\Experiments_V3\lwip_test\src\lwip\contrib\include" -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


