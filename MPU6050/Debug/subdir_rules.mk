################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
MPU6050.obj: ../MPU6050.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"D:/ccs/ccsv8/tools/compiler/ti-cgt-msp430_18.1.8.LTS/bin/cl430" -vmspx --code_model=large --data_model=restricted --near_data=none --use_hw_mpy=F5 --include_path="D:/ccs/ccsv8/ccs_base/msp430/include" --include_path="D:/ccscode/MPU6050" --include_path="D:/ccscode/MPU6050/driverlib/MSP430F5xx_6xx" --include_path="D:/ccs/ccsv8/tools/compiler/ti-cgt-msp430_18.1.8.LTS/include" --advice:power=all --define=__MSP430F5529__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="MPU6050.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"D:/ccs/ccsv8/tools/compiler/ti-cgt-msp430_18.1.8.LTS/bin/cl430" -vmspx --code_model=large --data_model=restricted --near_data=none --use_hw_mpy=F5 --include_path="D:/ccs/ccsv8/ccs_base/msp430/include" --include_path="D:/ccscode/MPU6050" --include_path="D:/ccscode/MPU6050/driverlib/MSP430F5xx_6xx" --include_path="D:/ccs/ccsv8/tools/compiler/ti-cgt-msp430_18.1.8.LTS/include" --advice:power=all --define=__MSP430F5529__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="main.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

msp430_oled.obj: ../msp430_oled.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"D:/ccs/ccsv8/tools/compiler/ti-cgt-msp430_18.1.8.LTS/bin/cl430" -vmspx --code_model=large --data_model=restricted --near_data=none --use_hw_mpy=F5 --include_path="D:/ccs/ccsv8/ccs_base/msp430/include" --include_path="D:/ccscode/MPU6050" --include_path="D:/ccscode/MPU6050/driverlib/MSP430F5xx_6xx" --include_path="D:/ccs/ccsv8/tools/compiler/ti-cgt-msp430_18.1.8.LTS/include" --advice:power=all --define=__MSP430F5529__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="msp430_oled.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

time.obj: ../time.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"D:/ccs/ccsv8/tools/compiler/ti-cgt-msp430_18.1.8.LTS/bin/cl430" -vmspx --code_model=large --data_model=restricted --near_data=none --use_hw_mpy=F5 --include_path="D:/ccs/ccsv8/ccs_base/msp430/include" --include_path="D:/ccscode/MPU6050" --include_path="D:/ccscode/MPU6050/driverlib/MSP430F5xx_6xx" --include_path="D:/ccs/ccsv8/tools/compiler/ti-cgt-msp430_18.1.8.LTS/include" --advice:power=all --define=__MSP430F5529__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="time.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


