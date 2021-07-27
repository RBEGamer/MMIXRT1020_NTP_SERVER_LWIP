################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/fsl_cache.c \
../drivers/fsl_clock.c \
../drivers/fsl_common.c \
../drivers/fsl_common_arm.c \
../drivers/fsl_enet.c \
../drivers/fsl_gpio.c \
../drivers/fsl_lpi2c.c \
../drivers/fsl_lpuart.c 

OBJS += \
./drivers/fsl_cache.o \
./drivers/fsl_clock.o \
./drivers/fsl_common.o \
./drivers/fsl_common_arm.o \
./drivers/fsl_enet.o \
./drivers/fsl_gpio.o \
./drivers/fsl_lpi2c.o \
./drivers/fsl_lpuart.o 

C_DEPS += \
./drivers/fsl_cache.d \
./drivers/fsl_clock.d \
./drivers/fsl_common.d \
./drivers/fsl_common_arm.d \
./drivers/fsl_enet.d \
./drivers/fsl_gpio.d \
./drivers/fsl_lpi2c.d \
./drivers/fsl_lpuart.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE -DCPU_MIMXRT1021DAG5A -DSDK_DEBUGCONSOLE=0 -DXIP_EXTERNAL_FLASH=1 -DXIP_BOOT_HEADER_ENABLE=1 -DLWIP_DISABLE_PBUF_POOL_SIZE_SANITY_CHECKS=1 -DSERIAL_PORT_TYPE_UART=1 -DMCUXPRESSO_SDK -DCPU_MIMXRT1021DAG5A_cm7 -DCR_INTEGER_PRINTF -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"/Users/marcelochsendorf/Documents/MCUXpressoIDE_11.4.0_6224/workspace/TEST_lwip_dhcp_bm/board" -I"/Users/marcelochsendorf/Documents/MCUXpressoIDE_11.4.0_6224/workspace/TEST_lwip_dhcp_bm/source" -I"/Users/marcelochsendorf/Documents/MCUXpressoIDE_11.4.0_6224/workspace/TEST_lwip_dhcp_bm/mdio" -I"/Users/marcelochsendorf/Documents/MCUXpressoIDE_11.4.0_6224/workspace/TEST_lwip_dhcp_bm/phy" -I"/Users/marcelochsendorf/Documents/MCUXpressoIDE_11.4.0_6224/workspace/TEST_lwip_dhcp_bm/drivers" -I"/Users/marcelochsendorf/Documents/MCUXpressoIDE_11.4.0_6224/workspace/TEST_lwip_dhcp_bm/lwip/port" -I"/Users/marcelochsendorf/Documents/MCUXpressoIDE_11.4.0_6224/workspace/TEST_lwip_dhcp_bm/lwip/src" -I"/Users/marcelochsendorf/Documents/MCUXpressoIDE_11.4.0_6224/workspace/TEST_lwip_dhcp_bm/lwip/src/include" -I"/Users/marcelochsendorf/Documents/MCUXpressoIDE_11.4.0_6224/workspace/TEST_lwip_dhcp_bm/utilities" -I"/Users/marcelochsendorf/Documents/MCUXpressoIDE_11.4.0_6224/workspace/TEST_lwip_dhcp_bm/device" -I"/Users/marcelochsendorf/Documents/MCUXpressoIDE_11.4.0_6224/workspace/TEST_lwip_dhcp_bm/component/uart" -I"/Users/marcelochsendorf/Documents/MCUXpressoIDE_11.4.0_6224/workspace/TEST_lwip_dhcp_bm/component/serial_manager" -I"/Users/marcelochsendorf/Documents/MCUXpressoIDE_11.4.0_6224/workspace/TEST_lwip_dhcp_bm/component/lists" -I"/Users/marcelochsendorf/Documents/MCUXpressoIDE_11.4.0_6224/workspace/TEST_lwip_dhcp_bm/xip" -I"/Users/marcelochsendorf/Documents/MCUXpressoIDE_11.4.0_6224/workspace/TEST_lwip_dhcp_bm/CMSIS" -O0 -fno-common -g3 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


