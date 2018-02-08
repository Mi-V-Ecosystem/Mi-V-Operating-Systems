################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/CoreUARTapb/core_uart_apb.c 

OBJS += \
./drivers/CoreUARTapb/core_uart_apb.o 

C_DEPS += \
./drivers/CoreUARTapb/core_uart_apb.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/CoreUARTapb/%.o: ../drivers/CoreUARTapb/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv64-unknown-elf-gcc -march=rv32im -mabi=ilp32 -msmall-data-limit=8 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -DNDEBUG -I"D:\dropbox\EW\miv-rv32im-freertos-port-test" -I"D:\dropbox\EW\miv-rv32im-freertos-port-test\drivers\CoreGPIO" -I"D:\dropbox\EW\miv-rv32im-freertos-port-test\drivers\CoreUARTapb" -I"D:\dropbox\EW\miv-rv32im-freertos-port-test\hal" -I"D:\dropbox\EW\miv-rv32im-freertos-port-test\riscv_hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


