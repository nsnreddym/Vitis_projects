################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lwip/api/api_lib.c \
../src/lwip/api/api_msg.c \
../src/lwip/api/err.c \
../src/lwip/api/if_api.c \
../src/lwip/api/netbuf.c \
../src/lwip/api/netdb.c \
../src/lwip/api/netifapi.c \
../src/lwip/api/sockets.c \
../src/lwip/api/tcpip.c 

OBJS += \
./src/lwip/api/api_lib.o \
./src/lwip/api/api_msg.o \
./src/lwip/api/err.o \
./src/lwip/api/if_api.o \
./src/lwip/api/netbuf.o \
./src/lwip/api/netdb.o \
./src/lwip/api/netifapi.o \
./src/lwip/api/sockets.o \
./src/lwip/api/tcpip.o 

C_DEPS += \
./src/lwip/api/api_lib.d \
./src/lwip/api/api_msg.d \
./src/lwip/api/err.d \
./src/lwip/api/if_api.d \
./src/lwip/api/netbuf.d \
./src/lwip/api/netdb.d \
./src/lwip/api/netifapi.d \
./src/lwip/api/sockets.d \
./src/lwip/api/tcpip.d 


# Each subdirectory must supply rules for building sources it contributes
src/lwip/api/%.o: ../src/lwip/api/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -IE:/Git_Repositories/Vitis_Workspace/Experiments_V3/Gigabit_Ethernet_Platform/export/Gigabit_Ethernet_Platform/sw/Gigabit_Ethernet_Platform/standalone_domain/bspinclude/include -I"E:\Git_Repositories\Vitis_Workspace\Experiments_V3\lwip_test\src\lwip\include" -I"E:\Git_Repositories\Vitis_Workspace\Experiments_V3\lwip_test\src\lwip\contrib\include" -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


