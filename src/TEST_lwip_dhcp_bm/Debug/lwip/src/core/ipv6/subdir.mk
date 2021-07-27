################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip/src/core/ipv6/dhcp6.c \
../lwip/src/core/ipv6/ethip6.c \
../lwip/src/core/ipv6/icmp6.c \
../lwip/src/core/ipv6/inet6.c \
../lwip/src/core/ipv6/ip6.c \
../lwip/src/core/ipv6/ip6_addr.c \
../lwip/src/core/ipv6/ip6_frag.c \
../lwip/src/core/ipv6/mld6.c \
../lwip/src/core/ipv6/nd6.c 

OBJS += \
./lwip/src/core/ipv6/dhcp6.o \
./lwip/src/core/ipv6/ethip6.o \
./lwip/src/core/ipv6/icmp6.o \
./lwip/src/core/ipv6/inet6.o \
./lwip/src/core/ipv6/ip6.o \
./lwip/src/core/ipv6/ip6_addr.o \
./lwip/src/core/ipv6/ip6_frag.o \
./lwip/src/core/ipv6/mld6.o \
./lwip/src/core/ipv6/nd6.o 

C_DEPS += \
./lwip/src/core/ipv6/dhcp6.d \
./lwip/src/core/ipv6/ethip6.d \
./lwip/src/core/ipv6/icmp6.d \
./lwip/src/core/ipv6/inet6.d \
./lwip/src/core/ipv6/ip6.d \
./lwip/src/core/ipv6/ip6_addr.d \
./lwip/src/core/ipv6/ip6_frag.d \
./lwip/src/core/ipv6/mld6.d \
./lwip/src/core/ipv6/nd6.d 


# Each subdirectory must supply rules for building sources it contributes
lwip/src/core/ipv6/%.o: ../lwip/src/core/ipv6/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE -DCPU_MIMXRT1021DAG5A -DSDK_DEBUGCONSOLE=0 -DXIP_EXTERNAL_FLASH=1 -DXIP_BOOT_HEADER_ENABLE=1 -DLWIP_DISABLE_PBUF_POOL_SIZE_SANITY_CHECKS=1 -DSERIAL_PORT_TYPE_UART=1 -DMCUXPRESSO_SDK -DCPU_MIMXRT1021DAG5A_cm7 -DCR_INTEGER_PRINTF -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"/Users/marcelochsendorf/Documents/MCUXpressoIDE_11.4.0_6224/workspace/TEST_lwip_dhcp_bm/board" -I"/Users/marcelochsendorf/Documents/MCUXpressoIDE_11.4.0_6224/workspace/TEST_lwip_dhcp_bm/source" -I"/Users/marcelochsendorf/Documents/MCUXpressoIDE_11.4.0_6224/workspace/TEST_lwip_dhcp_bm/mdio" -I"/Users/marcelochsendorf/Documents/MCUXpressoIDE_11.4.0_6224/workspace/TEST_lwip_dhcp_bm/phy" -I"/Users/marcelochsendorf/Documents/MCUXpressoIDE_11.4.0_6224/workspace/TEST_lwip_dhcp_bm/drivers" -I"/Users/marcelochsendorf/Documents/MCUXpressoIDE_11.4.0_6224/workspace/TEST_lwip_dhcp_bm/lwip/port" -I"/Users/marcelochsendorf/Documents/MCUXpressoIDE_11.4.0_6224/workspace/TEST_lwip_dhcp_bm/lwip/src" -I"/Users/marcelochsendorf/Documents/MCUXpressoIDE_11.4.0_6224/workspace/TEST_lwip_dhcp_bm/lwip/src/include" -I"/Users/marcelochsendorf/Documents/MCUXpressoIDE_11.4.0_6224/workspace/TEST_lwip_dhcp_bm/utilities" -I"/Users/marcelochsendorf/Documents/MCUXpressoIDE_11.4.0_6224/workspace/TEST_lwip_dhcp_bm/device" -I"/Users/marcelochsendorf/Documents/MCUXpressoIDE_11.4.0_6224/workspace/TEST_lwip_dhcp_bm/component/uart" -I"/Users/marcelochsendorf/Documents/MCUXpressoIDE_11.4.0_6224/workspace/TEST_lwip_dhcp_bm/component/serial_manager" -I"/Users/marcelochsendorf/Documents/MCUXpressoIDE_11.4.0_6224/workspace/TEST_lwip_dhcp_bm/component/lists" -I"/Users/marcelochsendorf/Documents/MCUXpressoIDE_11.4.0_6224/workspace/TEST_lwip_dhcp_bm/xip" -I"/Users/marcelochsendorf/Documents/MCUXpressoIDE_11.4.0_6224/workspace/TEST_lwip_dhcp_bm/CMSIS" -O0 -fno-common -g3 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


