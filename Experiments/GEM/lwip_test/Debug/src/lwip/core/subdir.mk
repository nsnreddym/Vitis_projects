################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lwip/core/altcp.c \
../src/lwip/core/altcp_alloc.c \
../src/lwip/core/altcp_tcp.c \
../src/lwip/core/def.c \
../src/lwip/core/dns.c \
../src/lwip/core/inet_chksum.c \
../src/lwip/core/init.c \
../src/lwip/core/ip.c \
../src/lwip/core/mem.c \
../src/lwip/core/memp.c \
../src/lwip/core/netif.c \
../src/lwip/core/pbuf.c \
../src/lwip/core/raw.c \
../src/lwip/core/stats.c \
../src/lwip/core/sys.c \
../src/lwip/core/tcp.c \
../src/lwip/core/tcp_in.c \
../src/lwip/core/tcp_out.c \
../src/lwip/core/timeouts.c \
../src/lwip/core/udp.c 

OBJS += \
./src/lwip/core/altcp.o \
./src/lwip/core/altcp_alloc.o \
./src/lwip/core/altcp_tcp.o \
./src/lwip/core/def.o \
./src/lwip/core/dns.o \
./src/lwip/core/inet_chksum.o \
./src/lwip/core/init.o \
./src/lwip/core/ip.o \
./src/lwip/core/mem.o \
./src/lwip/core/memp.o \
./src/lwip/core/netif.o \
./src/lwip/core/pbuf.o \
./src/lwip/core/raw.o \
./src/lwip/core/stats.o \
./src/lwip/core/sys.o \
./src/lwip/core/tcp.o \
./src/lwip/core/tcp_in.o \
./src/lwip/core/tcp_out.o \
./src/lwip/core/timeouts.o \
./src/lwip/core/udp.o 

C_DEPS += \
./src/lwip/core/altcp.d \
./src/lwip/core/altcp_alloc.d \
./src/lwip/core/altcp_tcp.d \
./src/lwip/core/def.d \
./src/lwip/core/dns.d \
./src/lwip/core/inet_chksum.d \
./src/lwip/core/init.d \
./src/lwip/core/ip.d \
./src/lwip/core/mem.d \
./src/lwip/core/memp.d \
./src/lwip/core/netif.d \
./src/lwip/core/pbuf.d \
./src/lwip/core/raw.d \
./src/lwip/core/stats.d \
./src/lwip/core/sys.d \
./src/lwip/core/tcp.d \
./src/lwip/core/tcp_in.d \
./src/lwip/core/tcp_out.d \
./src/lwip/core/timeouts.d \
./src/lwip/core/udp.d 


# Each subdirectory must supply rules for building sources it contributes
src/lwip/core/%.o: ../src/lwip/core/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -IE:/Git_Repositories/Vitis_Workspace/Experiments_V3/Gigabit_Ethernet_Platform/export/Gigabit_Ethernet_Platform/sw/Gigabit_Ethernet_Platform/standalone_domain/bspinclude/include -I"E:\Git_Repositories\Vitis_Workspace\Experiments_V3\lwip_test\src\lwip\include" -I"E:\Git_Repositories\Vitis_Workspace\Experiments_V3\lwip_test\src\lwip\contrib\include" -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


