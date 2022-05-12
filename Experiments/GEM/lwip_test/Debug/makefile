################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/lwip/netif/ppp/polarssl/subdir.mk
-include src/lwip/netif/ppp/subdir.mk
-include src/lwip/netif/subdir.mk
-include src/lwip/core/ipv6/subdir.mk
-include src/lwip/core/ipv4/subdir.mk
-include src/lwip/core/subdir.mk
-include src/lwip/contrib/subdir.mk
-include src/lwip/apps/snmp/subdir.mk
-include src/lwip/apps/smtp/subdir.mk
-include src/lwip/apps/netbiosns/subdir.mk
-include src/lwip/apps/mdns/subdir.mk
-include src/lwip/apps/altcp_tls/subdir.mk
-include src/lwip/api/subdir.mk
-include src/Ethernet/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
ELFSIZE += \
lwip_test.elf.size \


# All Target
all:
	$(MAKE) --no-print-directory pre-build
	$(MAKE) --no-print-directory main-build

# Main-build Target
main-build: lwip_test.elf secondary-outputs

# Tool invocations
lwip_test.elf: $(OBJS) ../src/lscript.ld $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM v7 gcc linker'
	arm-none-eabi-gcc -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -Wl,-build-id=none -specs=Xilinx.spec -Wl,-T -Wl,../src/lscript.ld -LE:/Git_Repositories/Vitis_projects/Experiments/GEM/Gigabit_Ethernet_Platform/export/Gigabit_Ethernet_Platform/sw/Gigabit_Ethernet_Platform/standalone_domain/bsplib/lib -o "lwip_test.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

lwip_test.elf.size: lwip_test.elf
	@echo 'Invoking: ARM v7 Print Size'
	arm-none-eabi-size lwip_test.elf  |tee "lwip_test.elf.size"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(S_UPPER_DEPS)$(C_DEPS)$(ELFSIZE) lwip_test.elf
	-@echo ' '

pre-build:
	-a9-linaro-pre-build-step
	-@echo ' '

secondary-outputs: $(ELFSIZE)

.PHONY: all clean dependents main-build pre-build

-include ../makefile.targets
