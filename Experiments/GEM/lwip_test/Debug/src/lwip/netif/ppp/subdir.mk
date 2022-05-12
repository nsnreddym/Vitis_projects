################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lwip/netif/ppp/auth.c \
../src/lwip/netif/ppp/ccp.c \
../src/lwip/netif/ppp/chap-md5.c \
../src/lwip/netif/ppp/chap-new.c \
../src/lwip/netif/ppp/chap_ms.c \
../src/lwip/netif/ppp/demand.c \
../src/lwip/netif/ppp/eap.c \
../src/lwip/netif/ppp/ecp.c \
../src/lwip/netif/ppp/eui64.c \
../src/lwip/netif/ppp/fsm.c \
../src/lwip/netif/ppp/ipcp.c \
../src/lwip/netif/ppp/ipv6cp.c \
../src/lwip/netif/ppp/lcp.c \
../src/lwip/netif/ppp/magic.c \
../src/lwip/netif/ppp/mppe.c \
../src/lwip/netif/ppp/multilink.c \
../src/lwip/netif/ppp/ppp.c \
../src/lwip/netif/ppp/pppapi.c \
../src/lwip/netif/ppp/pppcrypt.c \
../src/lwip/netif/ppp/pppoe.c \
../src/lwip/netif/ppp/pppol2tp.c \
../src/lwip/netif/ppp/pppos.c \
../src/lwip/netif/ppp/upap.c \
../src/lwip/netif/ppp/utils.c \
../src/lwip/netif/ppp/vj.c 

OBJS += \
./src/lwip/netif/ppp/auth.o \
./src/lwip/netif/ppp/ccp.o \
./src/lwip/netif/ppp/chap-md5.o \
./src/lwip/netif/ppp/chap-new.o \
./src/lwip/netif/ppp/chap_ms.o \
./src/lwip/netif/ppp/demand.o \
./src/lwip/netif/ppp/eap.o \
./src/lwip/netif/ppp/ecp.o \
./src/lwip/netif/ppp/eui64.o \
./src/lwip/netif/ppp/fsm.o \
./src/lwip/netif/ppp/ipcp.o \
./src/lwip/netif/ppp/ipv6cp.o \
./src/lwip/netif/ppp/lcp.o \
./src/lwip/netif/ppp/magic.o \
./src/lwip/netif/ppp/mppe.o \
./src/lwip/netif/ppp/multilink.o \
./src/lwip/netif/ppp/ppp.o \
./src/lwip/netif/ppp/pppapi.o \
./src/lwip/netif/ppp/pppcrypt.o \
./src/lwip/netif/ppp/pppoe.o \
./src/lwip/netif/ppp/pppol2tp.o \
./src/lwip/netif/ppp/pppos.o \
./src/lwip/netif/ppp/upap.o \
./src/lwip/netif/ppp/utils.o \
./src/lwip/netif/ppp/vj.o 

C_DEPS += \
./src/lwip/netif/ppp/auth.d \
./src/lwip/netif/ppp/ccp.d \
./src/lwip/netif/ppp/chap-md5.d \
./src/lwip/netif/ppp/chap-new.d \
./src/lwip/netif/ppp/chap_ms.d \
./src/lwip/netif/ppp/demand.d \
./src/lwip/netif/ppp/eap.d \
./src/lwip/netif/ppp/ecp.d \
./src/lwip/netif/ppp/eui64.d \
./src/lwip/netif/ppp/fsm.d \
./src/lwip/netif/ppp/ipcp.d \
./src/lwip/netif/ppp/ipv6cp.d \
./src/lwip/netif/ppp/lcp.d \
./src/lwip/netif/ppp/magic.d \
./src/lwip/netif/ppp/mppe.d \
./src/lwip/netif/ppp/multilink.d \
./src/lwip/netif/ppp/ppp.d \
./src/lwip/netif/ppp/pppapi.d \
./src/lwip/netif/ppp/pppcrypt.d \
./src/lwip/netif/ppp/pppoe.d \
./src/lwip/netif/ppp/pppol2tp.d \
./src/lwip/netif/ppp/pppos.d \
./src/lwip/netif/ppp/upap.d \
./src/lwip/netif/ppp/utils.d \
./src/lwip/netif/ppp/vj.d 


# Each subdirectory must supply rules for building sources it contributes
src/lwip/netif/ppp/%.o: ../src/lwip/netif/ppp/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -IE:/Git_Repositories/Vitis_Workspace/Experiments_V3/Gigabit_Ethernet_Platform/export/Gigabit_Ethernet_Platform/sw/Gigabit_Ethernet_Platform/standalone_domain/bspinclude/include -I"E:\Git_Repositories\Vitis_Workspace\Experiments_V3\lwip_test\src\lwip\include" -I"E:\Git_Repositories\Vitis_Workspace\Experiments_V3\lwip_test\src\lwip\contrib\include" -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


